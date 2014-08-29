#ifndef _DB_OPERATION_H_
#define _DB_OPERATION_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include "mysql.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "model.h"

using std::list;
using std::string;

class DBoperation {
private:
  std::string table_name;
  std::string table_user_name;
  std::string table_user_to_blog;
  std::string mysql_addr;
  std::string mysql_usr;
  std::string mysql_pwd;
  std::string mysql_database;
public:
  MYSQL my_connection;

  DBoperation(std::string mysql_addr, 
              std::string mysql_usr,
              std::string mysql_pwd, 
              std::string mysql_database);
  ~DBoperation();
  void DBConnect();
  bool DB_insertData(INSERT_DATA &insert_data);
  void SetTableName(const std::string table_name);
  void GetText(long startline, long length,
      std::list<std::list<std::string> > &result);
  void GetTables(std::list<std::string> &tables);
  void CreateTable();
  long Getcount();
  void DB_query(const char * sql_query, std::list<std::list<std::string> > &result);
  void GetWeiBos(long startline,long length,std::list<Blog> &weibos);
};

#endif
