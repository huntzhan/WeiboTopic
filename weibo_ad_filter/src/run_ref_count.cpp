/**
 * @file    run_ref_count.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/28/2014 09:28:10 AM
 * @version 1.0
 *
 */


#include <cstdlib>
#include <ctime>
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
using std::string;
using std::cout;
using std::endl;
using std::vector;

const unsigned ROWS_EACH_TIME = 8000;
DBpool insert;

inline bool IsMeaninglessBlog(ParsedBlog &pb);
inline bool IsSourceNotOk(const Blog &b);
void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas);

int main() {
  TextSpilt::init_ICTCAL();
  Parser parser;
  RefCount ref;
  std::list<ParsedBlog> parsed_blogs;

  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
  insert.DBinit("use filter_no_duplicate", connpool);

  // Allocator allo("Microblog1408215600");
  // if (allo.HasNextTable()) {
  Allocator allo(true);
  while (allo.HasNextTable()) {
    allo.NextTable();
    Log::Logging(RUN_T, "###Start Table " + allo.GetCurrentTableName() + ": " + std::to_string(allo.GetRowsOfCurrentTable()));
    while (allo.HasNextRow()) {
      std::list<Blog> blogs;
      unsigned count = allo.NextBlogs(ROWS_EACH_TIME, blogs);
      Log::Logging(RUN_T, "get rows from crawler: " + to_string(count));
      for(auto &blog : blogs) {
        if (blog.m_source == "微问")
          continue;
        /// Lexcal Analysis by ICTCLAS50
        std::vector<Word> words;
        parser.LexicalAnalysis(blog.m_content.c_str(), words);
        ParsedBlog pb(blog, words);
        if (IsMeaninglessBlog(pb))
          continue;
        unsigned fp = pb.fingerprint_();
        ref.AddFingerPrint(fp, 1);
        parsed_blogs.push_back(pb);
      }
    }
    std::vector<INSERT_DATA> insert_datas;
    for (auto i : parsed_blogs) {
      unsigned int pf;
      unsigned int count = ref.GetRefCount(i.fingerprint_(), 1, pf);
      // Log::Logging(REF_DIST_1_T, Blog2Str(i.blog_()) + ">" + to_string(pf) + ">" + to_string(count));
      if (count > 2)
        continue;
      /// source tactic
      if (IsSourceNotOk(i.blog_()))
        continue;
      insert_datas.push_back(i.ToInsertData());
    }
    InsertDataToTable(allo.GetCurrentTableName(), insert_datas);
    Log::Logging(RUN_T, "###Table " + allo.GetCurrentTableName() + " ends: >" + std::to_string(insert_datas.size()) + "/" + std::to_string(allo.GetRowsOfCurrentTable()));
    parsed_blogs.clear();
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
