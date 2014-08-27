/**
 * @file    main.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/26/2014 03:03:46 PM
 * @version 1.0
 *
 */
#include <map>
#include <sstream>
#include <iostream>
#include <vector>
#include "db/DBoperation.h"
#include "db/DBpool.h"
#include "db/connection_pool.h"
#include "db/model.h"
#include "split/parser.h"
#include "split/Textspilt.h"
#include "tactic/preprocessor.h"
#include "simhash/simhash.h"
#include "logger/log.h"
#include "ref_count/ref.h"
using std::to_string;
using std::vector;

#define SQL_ADDR "192.168.1.102"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"

Parser parser;
DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
DBpool insert;

INSERT_DATA PackInsertData(const Blog &b, const vector<Word> &words);
void FilterOneTable(string table, vector<INSERT_DATA> &insert_datas);
void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas);

vector<string> UnpackInsertData(const INSERT_DATA &data);
void AddSpecialToken(const string &m_content, vector<string> &words);
void RemoveSpecialToken(vector<string> &words);

void InitMain() {
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
  insert.DBinit("use filter", connpool);
  query.DBConnect();
  TextSpilt::init_ICTCAL();
}

std::set<string> GetProcessedTables(){
  std::set<string> done_tables;
  ifstream in("done.log");
  string done_table;
  while (in>>done_table) {
    string t = done_table.substr(8);
    done_tables.insert(t);
  }
  return done_tables;
}

std::list<string> GetNotProcessedTables() {
  std::list<string> tables;
  query.GetTables(tables);
  std::set<string> done_tables = GetProcessedTables();

  std::list<string> undone_tables;
  for(const string &table : tables){
    if(done_tables.find(table) == done_tables.end())
      undone_tables.push_back(table);
  }
  return undone_tables;
}

int main() {
  InitMain();
  Log::Logging(RUN_T, "Program Initialized");

  std::list<string> tables = GetNotProcessedTables();
  /// get tables from to be processed
  for(const string &table : tables){
    query.SetTableName(table);
    int number_all_rows = query.Getcount();
    Log::Logging(RUN_T, "Start Filtering One Table: " + to_string(number_all_rows));
    /// perform filtering on one table
    vector<INSERT_DATA> insert_datas;
    FilterOneTable(table, insert_datas);
    // Log::Logging(RUN_T, "Blogs in One Table After Filtering: " + 
    //     to_string(insert_datas.size()) + "/" + to_string(number_all_rows));
    /// Insert one table
    // string insert_tablename = "Filtered" + table;
    // InsertDataToTable(insert_tablename, insert_datas);
    // Log::Logging(RUN_T, "Insertions of " + insert_tablename + " ends");
  }

  Log::Logging(RUN_T, "Program finished");
  return EXIT_SUCCESS;
}

void FilterOneTable(string table, vector<INSERT_DATA> &insert_datas) {
  vector<INSERT_DATA> prepare_datas;
  query.SetTableName(table);
  int number_all_rows = query.Getcount();
  int number_left_rows = number_all_rows;
  int ROW_EACH_TIME = 2000;
  Preprocessor pre;
  while(number_left_rows > 0){
    std::list<Blog> weibos;
    int n = number_left_rows>ROW_EACH_TIME? ROW_EACH_TIME: number_left_rows;
    Log::Logging(RUN_T, "Get blogs from db: " + to_string(n));
    /// get blogs from db
    query.GetWeiBos(number_all_rows - number_left_rows, n, weibos);
    number_left_rows -= ROW_EACH_TIME;
    /// start blogs filtering
    for(auto &blog : weibos) {
      bool is_good_blog = pre.PerformTacticOnBlog(blog);
      /// Lexcal Analysis by ICTCLAS50
      std::vector<Word> words;
      parser.LexicalAnalysis(blog.m_content.c_str(), words);
      bool is_good_parsed_blog = pre.PerformTacticOnParsedBlog(words);
      if(is_good_blog && is_good_parsed_blog){
        /// change parsed blog into insert_data
        INSERT_DATA data = PackInsertData(blog, words);
        if(data.text.length()<2 || data.spilt.length()<2 || data.text.empty() || data.spilt.empty()){
          //std::cout << insertdata.text<<std::endl;
          ;
        }
        else
          prepare_datas.push_back(data);
      }
    }
  }
  /// ref count tactic
  RefCount ref;
  for(auto data: prepare_datas){
    /// ref count detection
    vector<string> w = UnpackInsertData(data);
    AddSpecialToken(data.text, w);  /// test symbols like http and @
    ref.AddFingerPrint(w);
    RemoveSpecialToken(w);
  }
  for(auto data: prepare_datas) {
    vector<string> w = UnpackInsertData(data);
    AddSpecialToken(data.text, w);  /// test symbols like http and @
    unsigned int count = ref.GetRefCount(w);
    Log::Logging(REFLOW_T, data.text + " > " + to_string(count));
  }
}

vector<string> UnpackInsertData(const INSERT_DATA &data) {
  vector<string> res;
  std::stringstream ss(data.spilt);
  string item;
  while(std::getline(ss, item, ' ')){
    res.push_back(item);
    std::getline(ss, item, ' ');
  }
  return res;
}

INSERT_DATA PackInsertData(const Blog &b, const vector<Word> &words) {
  string fenci;
  for (Word w : words){
    fenci.append(w.word + " " + w.proper + " ");
  }
  INSERT_DATA insertdata;
  insertdata.mid = b.m_mid;
  insertdata.text = b.m_content;
  insertdata.spilt = fenci;
  return insertdata;
}

/**
 * 这个是主要处理的 先从数据库里面提取数据，然后分词，最后插入
 */
void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas) {
  insert.SetTableName(tablename);
  /// 建立数据库的表
  insert.CreateTable();
  /// Perform DB Insertion
  insert.DB_insertData(insert_datas);
}

/**
 * @brief detect whether blog contains "@" or "http://" substring, 
 * if so, add rare charector to words
 */
void AddSpecialToken(const string &m_content, vector<string> &words) {
  if (m_content.find("@") != string::npos) {
    words.push_back("圙");
  }
  if (m_content.find("http://") != string::npos) {
    words.push_back("圚");
  }
}

void RemoveSpecialToken(vector<string> &words) {
  while (words.back() == "圙" || words.back() == "圚")
    words.pop_back();
}
