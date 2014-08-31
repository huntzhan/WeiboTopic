/**
 * @file    run_filter.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/31/2014 07:58:06 PM
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
#include "split/parser.h"
#include "db/model.h"
#include "db/parsedblog.h"
#include "ref_count/ref.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

const unsigned ROWS_EACH_TIME = 8000;
const unsigned HAMMING_DISTANCE = 1;
DBpool insert;

inline bool IsMeaninglessBlog(ParsedBlog &pb);
inline bool IsSourceNotOk(const Blog &b);
inline bool IsFromZombieUser(const Blog &b);
void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas);

int main(int argc, char **argv) {
  TextSpilt::init_ICTCAL();
  Parser parser;

  string db_name = "db_name";
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
  insert.DBinit("use " + db_name, connpool);

  string table_name(argv[1]);
  Allocator allo(table_name);
  if (allo.HasNextTable()) {
    RefCount ref;
    std::list<ParsedBlog> parsed_blogs;
    allo.NextTable();
    // Log::Logging(RUN_T, "###Start Table " + allo.GetCurrentTableName() + ": " + std::to_string(allo.GetRowsOfCurrentTable()));
    while (allo.HasNextRow()) {
      std::list<Blog> blogs;
      unsigned count = allo.NextBlogs(ROWS_EACH_TIME, blogs);
      // Log::Logging(RUN_T, "get rows from crawler: " + to_string(count));
      for(auto &blog : blogs) {
        if (IsSourceNotOk(blog))  /// source tacitc
          continue;
        /// Lexcal Analysis by ICTCLAS50
        std::vector<Word> words;
        parser.LexicalAnalysis(blog.m_content.c_str(), words);
        ParsedBlog pb(blog, words);
        if (IsMeaninglessBlog(pb))
          continue;
        unsigned fp = pb.fingerprint_();
        ref.AddFingerPrint(fp, 2);
        parsed_blogs.push_back(pb);
      }
    }
    std::vector<INSERT_DATA> insert_datas;
    for (auto i : parsed_blogs) {
      if (IsFromZombieUser(i.blog_()))  /// Zombie tactic
        continue;
      unsigned int pf;
      unsigned int count = ref.GetRefCount(i.fingerprint_(), HAMMING_DISTANCE, pf);
      // Log::Logging(REF_DIST_1_T, Blog2Str(i.blog_()) + ">" + to_string(pf) + ">" + to_string(count));
      if (count > 2)
        continue;
      insert_datas.push_back(i.ToInsertData());
    }
    string output_table_name = "Filtered" + allo.GetCurrentTableName();
    InsertDataToTable(output_table_name, insert_datas);
    cout << output_table_name << endl;
    // Log::Logging(RUN_T, "###Table " + allo.GetCurrentTableName() + " ends: >" + std::to_string(insert_datas.size()) + "/" + std::to_string(allo.GetRowsOfCurrentTable()));
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
      b.u_bi_followers_count*1.0 / b.u_followees < 0.4){  /// invalid zombie user
  // if (b.u_bi_followers_count*1.0 / b.u_followees < 0.5){  /// invalid zombie user
    return true;
  }
  return false;
}
