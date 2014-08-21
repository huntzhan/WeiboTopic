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

#include "mysql_driver.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <iostream>

using std::cout;
using std::endl;

using std::string;
using std::vector;
using sql::Driver;

namespace mysql_handler {

// BasicConnectionSetup.
// shared driver.
Driver *BasicConnectionSetup::driver_ =
  sql::mysql::get_mysql_driver_instance();
// end BasicConnectionSetup.


// SimpleConnectionSetup.
SharedConn SimpleConnectionSetup::RetrieveConnection() const {
  // let it crash.
  auto *con = driver_->connect(
      db_location_.url_,
      db_location_.username_,
      db_location_.password_);
  con->setSchema(db_location_.database_);
  return SharedConn(con);
}
// end SimpleConnectionSetup.


// BasicOperator.
void BasicOperator::Init() const {
  auto new_conn = set_current_conn();
  current_conn_ = std::move(new_conn);
}

SharedConn BasicOperator::current_conn() const {
  return current_conn_;
}
// end BasicOperator.


// SimpleOperator.
SimpleOperator::SimpleOperator(const std::string &db_name,
                               const std::string &table_name)
    : table_name_(table_name) {
  db_location_.url_ = "tcp://127.0.0.1:3306";
  db_location_.username_ = "root";
  db_location_.password_ = "123456";
  db_location_.database_ = db_name;
}


SharedConn SimpleOperator::set_current_conn() const {
  SimpleConnectionSetup conn_setup(db_location_);
  return conn_setup.RetrieveConnection();
}
// end SimpleOperator.

// TopicOperator
vector<string> TopicOperator::topic_for_test() {

  auto conn = current_conn();
  auto stmt = conn->createStatement();
  auto res = stmt->executeQuery(
      "SELECT topicid, topicwords\n"
      "FROM OneDayTopic");
  // fetch result.
  vector<string> results;
  while (res->next()) {
    results.push_back(
        res->getString("topicwords"));
  }
  return results;
}
// end TopicOperator

}  // namespace mysql_handler
