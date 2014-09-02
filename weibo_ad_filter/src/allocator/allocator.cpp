/**
 * @file    allocator.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/27/2014 09:43:47 PM
 * @version 1.0
 *
 */
#include <fstream>
#include <set>
#include "allocator.h"

/**
 *  @brief Constructor only get the tables starts from 'M'
 *
 *  @param start_table_no starts from 1
 */
Allocator::Allocator(bool is_start_over) {
  query.DBConnect();
  if (is_start_over)
    query.GetTables(tables);
  else
    tables = GetNotProcessedTables();
}

Allocator::Allocator(const string db_name, const string tablename) {
  query.DBConnect(db_name);
  std::list<string> all_tables;
  query.GetTables(all_tables);
  std::list<string>::iterator find_iter = std::find(all_tables.begin(), all_tables.end(), tablename);
  if (find_iter == all_tables.end()) {
    Log::Logging(RUN_T, tablename + " not exists in Allocator");
    exit(1);
  }
  tables.push_back(tablename);
}

/**
 *  @brief Constructor 
 *
 *  @param 
 */
Allocator::Allocator(const string tablename) {
  query.DBConnect();
  std::list<string> all_tables;
  query.GetTables(all_tables);
  std::list<string>::iterator find_iter = std::find(all_tables.begin(), all_tables.end(), tablename);
  if (find_iter == all_tables.end()) {
    Log::Logging(RUN_T, tablename + " not exists in Allocator");
    exit(1);
  }
  tables.push_back(tablename);
}

std::set<string> Allocator::GetProcessedTables(){
  std::set<string> done_tables;
  std::ifstream in("done.log");
  string done_table;
  while (in>>done_table) {
    string t = done_table.substr(8);
    done_tables.insert(t);
  }
  return done_tables;
}

std::list<string> Allocator::GetNotProcessedTables() {
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

int Allocator::NextBlogs(unsigned int num, list<Blog> &blogs) {
    unsigned n = rows_left_of_cur_table > num? 
      num: 
      rows_left_of_cur_table;
    query.GetWeiBos(rows_of_cur_table - rows_left_of_cur_table, n, blogs);
    rows_left_of_cur_table -= n;
    return n;
}

bool Allocator::HasNextTable() {
  if (! is_initialized) {
    return true;
  }
  cur_table++;
  if (tables.end() == cur_table--)
    return false;
  return true;
}

void Allocator::NextTable() {
  if (is_initialized == false) {
    cur_table = tables.begin();
    is_initialized = true;
  }
  else {
    cur_table++;
  }
  query.SetTableName(*cur_table);
  rows_of_cur_table = query.Getcount();
  rows_left_of_cur_table = rows_of_cur_table;
}

unsigned Allocator::GetRowsOfCurrentTable() {
  return rows_of_cur_table;
}

string Allocator::GetCurrentTableName() {
  return *cur_table;
}

bool Allocator::HasNextRow() {
  if (rows_left_of_cur_table > 0)
    return true;
  else return false;
}
