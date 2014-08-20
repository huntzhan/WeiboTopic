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
#include "cppconn/driver.h"
#include "cppconn/connection.h"

using std::string;
using sql::Driver;

namespace mysql_handler {

using SharedConn = std::shared_ptr<sql::Connection>;

// shared driver.
Driver *BasicConnectionSetup::driver_ =
  sql::mysql::get_mysql_driver_instance();

// just connect to a database.
SharedConn SimpleConnectionSetup::RetrieveConnection() const {
  const string url = "tcp://127.0.0.1:3306";
  const string username = "root";
  const string password = "123456";
  const string database = "test";

  // let it crash.
  auto *con = driver_->connect(url, username, password);
  con->setSchema(database);
  return SharedConn(con);
}

void BasicOperator::Init() const {
  auto new_conn = set_current_conn();
  current_conn_ = std::move(new_conn);
}

SharedConn SimpleOperator::set_current_conn() const {
  SimpleConnectionSetup conn_setup;
  return conn_setup.RetrieveConnection();
}

}  // namespace mysql_handler
