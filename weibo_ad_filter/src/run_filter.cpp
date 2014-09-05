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
bool IsDuplicateInsertTable(DBpool &insert,string table_name);

int main(int argc, char **argv) {
  TextSpilt::init_ICTCAL();
  Parser parser;

  argv[1]="./output.txt";
  argv[2]="filter_ref_zombie_source_bayes.Microblog1409824800.";

  /// get settings from argv
  ofstream out(argv[1]);
  string db_name, table_name;
  std::istringstream input(argv[2]);
  std::getline(input, db_name, '.');
  std::getline(input, table_name, '.');
  //防止重复插入

  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
  insert.DBinit("use " + db_name, connpool);

  ///重复的表就退出
  if(IsDuplicateInsertTable(insert,table_name)){
     return 0;
  }

  Allocator allo("sina", "Microblog1409824800");
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
      Log::Logging(RUN_T, Blog2Str(i.blog_()) );
    }
    string output_table_name = "Filtered" + allo.GetCurrentTableName();
  //  InsertDataToTable(output_table_name, insert_datas);
    out << db_name << "." << output_table_name << endl;
    out.close();
    Log::Logging(RUN_T, "###Table " + allo.GetCurrentTableName() + " ends: >" + std::to_string(insert_datas.size()) + "/" + std::to_string(allo.GetRowsOfCurrentTable()));
  }
}
bool IsDuplicateInsertTable(DBpool &insert,string table_name){
  std::list<std::string> tables;
  string output_table_name="Filtered"+table_name;
  insert.GetTables(tables);
  cout<<tables.size()<<endl;
  for(std::string table:tables){
	  if(table==output_table_name){
		  cout<<"duplicate table "<< table<<endl;
		  return true;
	  }
  }
  return false;
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
      b.m_source == "唱吧" ||
      b.m_source == "微游戏" ||
      b.m_source == "微音乐" ||
      b.m_source == "分享按钮" ||
      b.m_source == "应用广场" ||
      b.m_source == "百度分享" ||
      b.m_source == "秦时明月手游" ||
      b.m_source == "实惠" ||
      b.m_source == "陌陌" ||
      b.m_source == "股票123个股雷" ||
      b.m_source == "微盘" ||
      b.m_source == "互粉大厅" ||
      b.m_source == "Instagram" ||
      b.m_source == "美团网分享" ||
      b.m_source == "微视" ||
      b.m_source == "扇贝网" ||
      b.m_source == "蜻蜓.fm收音机" ||
      b.m_source == "QQ音乐" ||
      b.m_source == "快手" ||
      b.m_source == "虾米音乐" ||
      b.m_source == "微相册" ||
      b.m_source == "微三国" ||
      b.m_source == "千图网免费素材" ||
      b.m_source == "美团网" ||
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
  // if (b.u_bi_followers_count*1.0 / b.u_followees < 0.5){  /// invalid zombie user
    return true;
  }
  return false;
}
