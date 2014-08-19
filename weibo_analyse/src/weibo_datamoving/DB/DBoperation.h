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

typedef struct _insert_data {
  std::string mid;
  std::string text;
  std::string spilt;
} INSERT_DATA;

class DBoperation {
private:
  std::string table_name;
  std::string mysql_addr;
  std::string mysql_usr;
  std::string mysql_pwd;
  std::string mysql_database;
public:
  MYSQL my_connection;

  void DBinit(std::string mysql_addr, std::string mysql_usr,
      std::string mysql_pwd, std::string mysql_database) {
    this->mysql_addr = mysql_addr;
    this->mysql_usr = mysql_usr;
    this->mysql_pwd = mysql_pwd;
    this->mysql_database = mysql_database;

  }

  void DBConnect();
  bool DB_insertData(INSERT_DATA &insert_data);
  void SetTableName(const std::string table_name);
  void GetText(long startline, long length,
      std::list<std::list<std::string> > &result);
  void GetTables(std::list<std::string> &tables);
  void CreateTable();
  long Getcount();
  void DB_query(const char * sql_query, std::list<std::list<std::string> > &result);
};

#endif
