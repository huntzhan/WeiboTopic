/**
 * @file    allocator.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/27/2014 09:33:14 PM
 * @version 1.0
 *
 */

#ifndef allocator_INC
#define allocator_INC

#include <map>
#include <sstream>
#include <vector>
#include <utility>
#include <list>
#include <algorithm>
#include "db/DBoperation.h"
#include "db/model.h"
#include "logger/log.h"
using std::to_string;
using std::vector;

#define SQL_ADDR "192.168.1.108"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"
static DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
/**
 *  @brief Allocator 
 */
class Allocator {
  public:
    int NextBlogs(unsigned int num, list<Blog> &blogs);
    unsigned GetRowsOfCurrentTable();
    string GetCurrentTableName();
    bool HasNextRow();
    bool HasNextTable();
    void NextTable();

    // ====================  LIFECYCLE     ==================================
    Allocator(const string tablename);
    Allocator(const string db_name, const string tablename);
    Allocator(bool is_start_over);
    ~Allocator(){}

  private:
    std::list<string> GetNotProcessedTables();
    std::set<string> GetProcessedTables();

    std::list<string> tables;
    std::list<string>::iterator cur_table;
    unsigned rows_of_cur_table;
    unsigned rows_left_of_cur_table;
    bool is_initialized = false;
    // DISALLOW_COPY_AND_ASSIGN
    Allocator(const Allocator&);
    void operator=(const Allocator&);
};
#endif
