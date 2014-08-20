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

// BasicConnectionSetup.
// shared driver.
Driver *BasicConnectionSetup::driver_ =
  sql::mysql::get_mysql_driver_instance();
// end BasicConnectionSetup.

// SimpleConnectionSetup.
SimpleConnectionSetup::SimpleConnectionSetup(
    string url,
    string username,
    string password,
    string database)
    : url_(url),
      username_(username), password_(password),
      database_(database) {/* empty */}

SharedConn SimpleConnectionSetup::RetrieveConnection() const {
  // let it crash.
  auto *con = driver_->connect(url_, username_, password_);
  con->setSchema(database_);
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
SharedConn SimpleOperator::set_current_conn() const {
  SimpleConnectionSetup conn_setup;
  return conn_setup.RetrieveConnection();
}
// end SimpleOperator.

}  // namespace mysql_handler
