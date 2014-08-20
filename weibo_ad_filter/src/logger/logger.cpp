/**
 * @file    logger.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/20/2014 12:03:29 PM
 * @version 1.0
 *
 */

#include "logger.h"
#include "db/DBoperation.h"

/**
 *  @brief Constructor
 *
 *  @param
 */

Logger::Logger(string path) {
  /// out.open(path);

  DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
  query.SetTableName(tablename);
  this->number_all_rows = query.Getcount();
}

Blog Logger::NextBlog(){
  if(cached_blogs.size() == 0){
    DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
    query.SetTableName(tablename);
    int n = this->number_left_rows>ROW_EACH_TIME? ROW_EACH_TIME: this->number_left_rows;
    query.GetWeiBos(this->number_all_rows - this->number_left_rows, n, cached_blogs);
    this->number_left_rows -= ROW_EACH_TIME;
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
