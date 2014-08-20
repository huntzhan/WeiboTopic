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

#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"

using std::string;
using std::shared_ptr;
using std::make_shared;
using sql::Driver;
using sql::Connection;

namespace mysql_handler {

// init pointer.
Driver *ConnectionSetup::driver_ = sql::mysql::get_mysql_driver_instance();

// @brief: make connection.
shared_ptr<Connection> ConnectionSetup::RetrieveConnection() {
  const string url = "tcp://127.0.0.1:3306";
  const string username = "root";
  const string password = "123456";

  Connection *con = ConnectionSetup::driver_->connect(url, username, password);
  return shared_ptr<Connection>(con);
}

}  // namespace mysql_handler
