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


void BasicHandler::Init() {
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


SharedConn SimpleHandler::set_current_conn() {
  SimpleConnectionSetup conn_setup(db_location_);
  return conn_setup.RetrieveConnection();
}


string SimpleHandler::table_name() const {
  return table_name_;
}


vector<string> TopicHandler::GetMessages() {

  auto conn = current_conn();
  // create sql.
  ostringstream formated_sql;
  formated_sql << "SELECT content\n"
               << "From " << table_name();
  // make query.
  unique_ptr<Statement> stmt(conn->createStatement());
  unique_ptr<ResultSet> res(
      stmt->executeQuery(formated_sql.str()));
  // fetch result.
  vector<string> results;
  while (res->next()) {
    results.push_back(res->getString("content"));
  }
  return results;
}


}  // namespace mysql_handler
