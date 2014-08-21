/**
 * @file    logger.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/20/2014 12:03:29 PM
 * @version 1.0
 *
 */

#include "logger.h"
#include "db/DBoperation.h"
#include <iostream>
#include <vector>
#include <algorithm>
using std::cout;
using std::endl;
using std::vector;

/**
 *  @brief Constructor
 *
 *  @param
 */

Logger::Logger(string path) {
  out.open(path);
  DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
  query.DBConnect();
  query.SetTableName(tablename);
  this->number_all_rows = query.Getcount();
  this->number_left_rows = number_all_rows;
  TextSpilt::init_ICTCAL();
  cout<<"#####Logger Initialized, Time taken: "<<difftime(time(NULL), t_start)<<endl;
  cout<<"#####"<<number_all_rows<<endl;
}

void Logger::AddRemovedBlog(const Blog &b) {
  if(rand() % 100 == 1)  // output with probability of %1
    out<<b.m_content<<endl<<"#####"<<endl;
  this->number_removed_rows++;
  // std::vector<Word> words;
  // parser.LexicalAnalysis(b.m_content, words);
  // for(const auto &w : words)
  //     ++word_count[w.word];
}

void Logger::ProduceFinalReport() {
  cout<<"#####Report about table: "<<tablename<<endl;
  cout<<"#####Removed blogs number: "<<this->number_all_rows<<endl;
  cout<<"#####All blogs number: "<<this->number_removed_rows<<endl;
  cout<<"#####Removed blogs / All blogs: %"<<(this->number_removed_rows*1.0 / this->number_all_rows * 100)<<endl;

  // vector<std::pair<string, size_t>> vsort(word_count.size());
  // for(const auto &p : word_count)
  //   vsort.push_back(p);
  // std::sort(vsort.begin(), vsort.end(), sort_pair_second());
  // cout<<"#####Frequent Spam Words: "<<endl;
  // cout.width(20);
  // for(const auto &p : vsort){
  //   cout<<std::left<<p.first<<std::left<<p.second<<endl;
  // }

  cout<<"#####Report ends..."<<endl;
}

void Logger::AskDataFromDB() {
  DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
  query.DBConnect();
  query.SetTableName(tablename);
  int n = this->number_left_rows>ROW_EACH_TIME? ROW_EACH_TIME: this->number_left_rows;
  query.GetWeiBos(this->number_all_rows-this->number_left_rows, n, cached_blogs);
  this->number_left_rows -= ROW_EACH_TIME;
  cout<<"#####Processed: %"<<((number_all_rows-number_left_rows)*1.0 / number_all_rows * 100)
    <<", Time taken: "<<difftime(time(NULL), t_start)<<endl;
}

Blog Logger::NextBlog(){
  if(cached_blogs.size() == 0){
    AskDataFromDB();
  }
  Blog b = cached_blogs.back();
  cached_blogs.pop_back();
  return b;
}

bool Logger::HasNextBlog() {
  if (number_left_rows > 0)
    return true;
  return false;
}

/**
 *  @brief Destructor
 *
 *  @param
 */
Logger::~Logger() {
  /// out.close();
}
