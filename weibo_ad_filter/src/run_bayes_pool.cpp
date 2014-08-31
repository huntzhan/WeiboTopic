/**
 * @file    run_bayes_pool.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/31/2014 03:26:17 PM
 * @version 1.0
 *
 */

#include <cstdlib>
#include <ctime>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "db/DBpool.h"
#include "db/connection_pool.h"
#include "allocator/allocator.h"
#include "logger/log.h"
#include "split/parser.h"
#include "db/model.h"
#include "db/parsedblog.h"
#include "ref_count/ref.h"
#include "bayes/bayes_classifier.h"
#include "bayes/bayes_factory.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

const unsigned ROWS_EACH_TIME = 8000;
const unsigned HAMMING_DISTANCE = 1;
const double BAYES_THRESHOLD = 0.7;
const unsigned REF_COUNT_LOWER_BOUND = 25;
DBpool insert;

inline bool IsMeaninglessBlog(ParsedBlog &pb);
inline bool IsSourceNotOk(const Blog &b);
inline bool IsFromZombieUser(const Blog &b);
void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas);
void MakeWordsCounter(std::list<shared_ptr<ParsedBlog>> &parsed_blogs, std::shared_ptr<std::map<string, double>> &word_dict);

int main() {
  TextSpilt::init_ICTCAL();
  Parser parser;

  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
  // insert.DBinit("use filter_no_duplicate", connpool);
  insert.DBinit("use filter_ref_zombie_source_bayes", connpool);

  // Allocator allo("Microblog1408060800");
  // if (allo.HasNextTable()) {
  Allocator allo(false);
  while (allo.HasNextTable()) {
    std::list<shared_ptr<ParsedBlog>> parsed_blogs;
    allo.NextTable();
    Log::Logging(RUN_T, "###Start Table " + allo.GetCurrentTableName() + ": " + std::to_string(allo.GetRowsOfCurrentTable()));
    unsigned source_count = 0;
    while (allo.HasNextRow()) {
      std::list<Blog> blogs;
      unsigned count = allo.NextBlogs(ROWS_EACH_TIME, blogs);
      Log::Logging(RUN_T, "get rows from crawler: " + to_string(count));
      for(auto &blog : blogs) {
        if (IsSourceNotOk(blog)) {
          source_count++;
          continue;
        }
        /// Lexcal Analysis by ICTCLAS50
        std::vector<Word> words;
        parser.LexicalAnalysis(blog.m_content.c_str(), words);
        shared_ptr<ParsedBlog> pb(new ParsedBlog(blog, words));
        if (IsMeaninglessBlog(*pb))
          continue;
        parsed_blogs.push_back(pb);
      }
    }
    /// RefCount
    RefCount ref;
    for (auto &blog : parsed_blogs) {
      unsigned fp = pb->fingerprint_();
      unsigned match_fp = ref.AddFingerPrint(fp, HAMMING_DISTANCE);
      pb->set_match_fingerprint(match_fp);
    }
    /// Make Bayes Classifiers
    BayesFactory bayes_factory;
    auto bayes_classifiers = bayes_factory.MakeBayesClassifiers(parsed_blogs);
    BayesPool bayes_pool;
    for (auto &bayes : bayes_classifiers) {
      if (bayes.GetSizeOfKeyWords() < 2)
        continue;
      bayes_pool.AddBayesClassifier(bayes);
    }
    /// filter blogs
    std::vector<INSERT_DATA> insert_datas;
    unsigned ref_left_count = 0;
    unsigned bayes_count = 0;
    unsigned zombie_count = 0;
    for (auto &ppb : parsed_blogs) {
      unsigned int pf;
      unsigned int count = ref.GetRefCount(ppb->fingerprint_(), 2, pf);
      if (count >= 5)
        continue;
      ref_left_count++;
      bool is_spam_bayes= false;
      double prob;
      for (auto &bayes : classifiers) {
        if (bayes->IsSpam(*ppb, prob)) {
          is_spam_bayes = true;
          break;
        }
      }
      if (is_spam_bayes) {
        bayes_count++;
        // Log::Logging(BAYES_DUMP_T, Blog2Str(ppb->blog_()) + ">" + to_string(prob));
        continue;
      }
      else {
        // Log::Logging(BAYES_PASS_T, Blog2Str(ppb->blog_()) + ">" + to_string(prob));
      }
      if (IsFromZombieUser(ppb->blog_())) {
        zombie_count++;
        continue;
      }
      insert_datas.push_back(ppb->ToInsertData());
    }
    /// persistence
    string table_name_for_insert = "Filtered" + allo.GetCurrentTableName();
    InsertDataToTable(table_name_for_insert, insert_datas);
    Log::Logging(RUN_T, "###Number of blogs filtered by source: " + std::to_string(source_count)); 
    Log::Logging(RUN_T, "###Number of blogs filtered by Bayes: " + std::to_string(bayes_count) + "/"  + std::to_string(ref_left_count)); 
    Log::Logging(RUN_T, "###Number of blogs filtered by zombie: " + std::to_string(zombie_count) + "/"  + std::to_string(ref_left_count)); 
    Log::Logging(RUN_T, "###Table " + table_name_for_insert + " ends: >" + std::to_string(insert_datas.size()) + "/" + std::to_string(allo.GetRowsOfCurrentTable()));
  }
}

bool IsMeaninglessBlog(ParsedBlog &pb) {
  const std::vector<Word> &Ws = pb.Words_();
  const Blog &blog = pb.blog_();
  if (Ws.size() == 1 && Ws.front().word.empty()) {
    return true;
  }
  else if (Ws.size() <= 3) {
    // Log::Logging(TOO_SHORT_T, std::to_string(Ws.size()) + " " + blog.m_content);
    return true;
  }
  else return false;
}

void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas) {
  insert.SetTableName(tablename);
  insert.CreateTable();
  insert.DB_insertData(insert_datas);
}

inline bool IsSourceNotOk(const Blog &b) {
  if (b.m_source == "微问" ||
      b.m_source == "爱问知识人" ||
      b.m_source == "新浪博客" ||
      b.m_source == "皮皮时光机" ||
      b.m_source == "投票" ||
      b.m_source == "淘宝网" ||
      b.m_source == "幸运抽金条" ||
      b.m_source == "格瓦拉电影" ||
      b.m_source == "美拍" ||
      b.m_source == "美团网" ||
      b.m_source == "无线会员中心" ||
      b.m_source == "微博相册" ||
      b.m_source == "天天动听音乐播放器")
    return true;
  return false;
}

inline bool IsFromZombieUser(const Blog &b) {
  if (b.u_vierfied)
    return false;
  if (b.u_fans <= 50 || 
      b.u_followees >= 1000 || 
      b.u_bi_followers_count*1.0 / b.u_followees < 0.2){  /// invalid zombie user
    return true;
  }
  return false;
}

