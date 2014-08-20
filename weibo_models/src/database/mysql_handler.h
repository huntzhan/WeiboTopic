// ============================================================================
//
//       Filename:  mysql_handler.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/20/2014 09:46:26
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (zhx), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <memory>

#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"

#ifndef _DATABASE_MYSQL_HANDLER_H_
#define _DATABASE_MYSQL_HANDLER_H_
namespace mysql_handler {

class ConnectionSetup {
 public:
  std::shared_ptr<sql::Connection> RetrieveConnection();

 private:
  static sql::Driver *driver_;
};

}  // namespace mysql_handler
#endif
