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
// end BasicOperator.


// SimpleOperator.
SimpleOperator::SimpleOperator(const string &db_name) {
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

}  // namespace mysql_handler
