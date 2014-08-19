/*
 * DBpool.cpp
 *
 *  Created on: 2014年8月17日
 *      Author: jeffzhuang
 */
#include "DBpool.h"

void DBpool::DBinit(std::string database, ConnPool *connpool) {
  m_connpool = connpool;
  ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "", 50);
  con = m_connpool->GetConnection();
  state = con->createStatement();
  state->execute(database);

}
void DBpool::DBclose() {
  m_connpool->ReleaseConnection(con);
}

void DBpool::SetTableName(const std::string table_name) {
  this->table_name = table_name;
}
/**
 * 获得标的总行数
 */
long DBpool::GetTablecount() {
  ResultSet *result;
  string str;
  char sql_query[100];
  sprintf(sql_query, "select count(*) from %s ", table_name.c_str());

  try {
    result = state->executeQuery(sql_query);
    while (result->next()) {
      str = result->getString(1);
    }
  } catch (sql::SQLException&e) {
    perror(e.what());
  }

  long count = atol(str.c_str());
  return count;
}

/**
 * 查询数据库下面有多少张表
 */
void DBpool::ShowTable(std::list<std::string> &tables) {
  ResultSet *result;

  try {
    result = state->executeQuery("show tables");
    while (result->next()) {
      tables.push_back(result->getString(1));
    }
  } catch (sql::SQLException&e) {
    perror(e.what());
  }
}

DBpool::~DBpool() {
  DBclose();
}
/**
 *创建表格 具体创建什么表 可以修改sql_query
 */
void DBpool::CreateTable() {
  int res;
  char sql_query[1024];
  sprintf(sql_query,
      "  CREATE TABLE `%s` (`mid` varchar(50) NOT NULL, `text` varchar(250) DEFAULT NULL, `spilt` varchar(1000) DEFAULT NULL,  PRIMARY KEY (`mid`)) ENGINE=InnoDB DEFAULT CHARSET=utf8",
      table_name.c_str());

  try {
    state->execute(sql_query);

  } catch (sql::SQLException&e) {
    perror(e.what());
  }
}
/*
 *从数据库获得文本内容 
 *
 */
void DBpool::GetText(long startline, long length,
    std::list<std::list<std::string> > &result) {

  char sql_query[200];
  sprintf(sql_query, "select mid,content from %s limit %ld ,%ld ",
      table_name.c_str(), startline, length);

  ResultSet *resultset;
  try {
    resultset = state->executeQuery(sql_query);
    while (resultset->next()) {
      std::list<std::string> list;
      list.push_back(resultset->getString(1));
      list.push_back(resultset->getString(2));
      result.push_back(list);
    }
  } catch (sql::SQLException&e) {
    perror(e.what());
  }
}

void DBpool::GetTables(std::list<std::string> &tables) {
  std::list<std::list<std::string> > result;
  ResultSet *resultset;
  try {
    resultset = state->executeQuery("show tables");
    while (resultset->next()) {
      std::string table = resultset->getString(1);
      if (table[0] == 'M') {
        std::cout << table << std::endl;
        tables.push_back(table);
      }
    }
  } catch (sql::SQLException&e) {
    perror(e.what());
  }
}

bool DBpool::DB_insertData(std::vector<INSERT_DATA> &insert_data) {
  char goodtext[1024];
  try {
    con->setAutoCommit(false);
    char sql_query[1024];
    std::vector<INSERT_DATA>::iterator it_insert = insert_data.begin();
    std::vector<INSERT_DATA>::iterator end_insert = insert_data.end();
    for (; it_insert != end_insert; it_insert++) {
      const char *row = it_insert->text.c_str();
      mysql_escape_string(goodtext, row, strlen(row));
      sprintf(sql_query, "insert into %s values('%s','%s','%s')",
          table_name.c_str(), it_insert->mid.c_str(), goodtext,
          it_insert->spilt.c_str());
      state->executeUpdate(sql_query);
    }
    con->commit();
    con->setAutoCommit(true);
  } catch (sql::SQLException&e) {
    perror(e.what());
  }

  return false;
}
