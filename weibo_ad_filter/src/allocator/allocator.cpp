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
 *  @brief Constructor
 *
 *  @param start_table_no starts from 1
 */
Allocator::Allocator(unsigned start_table_no, bool is_start_over) {
  query.DBConnect();
  if (is_start_over)
    query.GetTables(tables);
  else
    tables = GetNotProcessedTables();
  cur_table = tables.begin();
  for (unsigned i= 1; i<start_table_no; i++)
    cur_table++;
  query.SetTableName(*cur_table);
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

int Allocator::GetBlogs(unsigned int num, list<Blog> &blogs) {
    unsigned n = rows_of_cur_table > num? 
      num: 
      rows_left_of_cur_table;
    query.GetWeiBos(rows_of_cur_table - rows_left_of_cur_table, n, blogs);
    rows_left_of_cur_table -= n;
    return n;
}

bool Allocator::HasNextTable() {
  if (tables.end() == cur_table)
    return false;
  else return true;
}
  
void Allocator::NextTable() {
  cur_table++;
  query.SetTableName(*cur_table);
  rows_of_cur_table = query.Getcount();
  rows_left_of_cur_table = rows_of_cur_table;
}

unsigned Allocator::GetRowsOfCurrentTable() {
  return rows_of_cur_table;
}
