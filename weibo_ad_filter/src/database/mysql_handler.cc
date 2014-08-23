// ============================================================================
//
//       Filename:  mysql_handler.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/20/2014 10:16:36
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "database/mysql_handler.h"

#include <string>
#include <memory>
#include <utility>
#include <sstream>
#include <iostream>

#include "mysql_driver.h"

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"

using std::string;
using std::ostringstream;
using std::vector;
using std::unique_ptr;
using sql::Driver;
using sql::Statement;
using sql::ResultSet;
using std::cout;
using std::endl;

namespace mysql_handler {

// shared driver.
Driver *BasicConnectionSetup::driver_ =
  sql::mysql::get_mysql_driver_instance();


SharedConn SimpleConnectionSetup::RetrieveConnection() const {
  // let it crash.
  auto *con = driver_->connect(
      db_location_.url_,
      db_location_.username_,
      db_location_.password_);
  con->setSchema(db_location_.database_);
  return SharedConn(con);
}


void BasicHandler::Init() const {
  auto new_conn = set_current_conn();
  current_conn_ = std::move(new_conn);
}

SharedConn BasicHandler::current_conn() const {
  return current_conn_;
}


SimpleHandler::SimpleHandler(const std::string &db_name,
                             const std::string &table_name)
    : table_name_(table_name) {
  db_location_.url_ = "tcp://127.0.0.1:3306";
  db_location_.username_ = "root";
  db_location_.password_ = "123456";
  db_location_.database_ = db_name;
}


SharedConn SimpleHandler::set_current_conn() const {
  SimpleConnectionSetup conn_setup(db_location_);
  return conn_setup.RetrieveConnection();
}


string SimpleHandler::table_name() const {
  return table_name_;
}

int SpamHandler::AddSpams(vector<unsigned int> &spams) {
  /// timer
  time_t t_start = time(NULL);

  auto conn = current_conn();
  // create sql.
  ostringstream formated_sql;
  formated_sql << "INSERT INTO spam\n"
               << "(fingerprint)\n"
               << "VALUES\n";
  for(auto ib = spams.cbegin(), ie = spams.cend(); ib != ie; ib++){
    if(ib == ie-1)
      formated_sql<< "(" << *ib << ")\n";  /// no comma needed for last one
    else
      formated_sql<< "(" << *ib << "),\n";
  }
  // make query.
  try {
    unique_ptr<Statement> stmt;
    stmt.reset(conn->createStatement());
    int update_count = stmt->executeUpdate(formated_sql.str());
    /// timer
    time_t t_end = time(NULL);
    /// cout<<"###MYSQL adding "<< update_count << " spams takes time " << (t_end - t_start) << " seconds" <<endl;
    return update_count;
  } catch(sql::SQLException &ex) {
    cout<<"SQL Error: " << ex.what() << endl;
    cout<<"SQL Syntax: " << formated_sql.str() << endl;
    abort();
  }
  return -1;
}

bool SpamHandler::QuerySpamSimhash(vector<unsigned int> &spam) {
  if (spam.empty())
    return false;
  /// timer
  time_t t_start = time(NULL);

  auto conn = current_conn();
  // create sql.
  ostringstream formated_sql;
  formated_sql << "SELECT EXISTS( SELECT *\n"
               << "From " << table_name() << "\n"
               << "WHERE \n";
  for(auto v : spam) {
    formated_sql<< "fingerprint = " << v << "\nOR ";
  }
  formated_sql<<"0)";  /// make up for ended OR
  // make query.
  try {
    unique_ptr<Statement> stmt;
    unique_ptr<ResultSet> res;
    stmt.reset(conn->createStatement());
    res.reset(stmt->executeQuery(formated_sql.str()));
    // fetch result.
    int exist = -1;
    if (res->next())
      exist = res->getInt(1);  /// first column
    /// timer
    time_t t_end = time(NULL);
    /// cout<<"###MYSQL querying "<< spam.size() << " simhash values takes time " << (t_end - t_start) << " seconds" <<endl;
    return exist;
  } catch (sql::SQLException &ex) {
    cout<< "SQL Error: " << ex.what() << endl;
    cout<<"SQL Syntax: " << formated_sql.str() << endl;
    abort();
  }
}

}  // namespace mysql_handler
