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

#include <string>
#include <memory>

#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"

#ifndef _DATABASE_MYSQL_HANDLER_H_
#define _DATABASE_MYSQL_HANDLER_H_
namespace mysql_handler {

using SharedConn = std::shared_ptr<sql::Connection>;

// @brief: Setting up connection to database. Pointer to Connection object
// would be returned as a shared_ptr, hence you should not manually free such
// pointer.
class BasicConnectionSetup {
 public:
  // @return: shared pointer to sql::Connection object configured by
  // (url, username, password, database). 
  virtual SharedConn RetrieveConnection() const = 0;
  static sql::Driver *driver_;
};

// @brief: Connect to a specific database.
class SimpleConnectionSetup : public BasicConnectionSetup {
 public:
  // use default values.
  SimpleConnectionSetup() = default;
  // constructor that use (url, username, password, database) to target db.
  SimpleConnectionSetup(
      std::string url,
      std::string username,
      std::string password,
      std::string database);

  // interface.
  SharedConn RetrieveConnection() const override;

 private:
  // default values of (url, username, password, database).
  const std::string url_ = "tcp://127.0.0.1:3306";
  const std::string username_ = "root";
  const std::string password_ = "123456";
  const std::string database_ = "test";
};

// @brief: Use BasicConnectionSetup and its derived class to open connection.
// Then use such connection to carried out operations.
class BasicOperator {
 public:
  // @brief: setup connection.
  virtual void Init() const final;  // forbids override.
  virtual SharedConn set_current_conn() const = 0;

 private:
  mutable SharedConn current_conn_ = nullptr;
};

class SimpleOperator : public BasicOperator {
 public:
  SharedConn set_current_conn() const override;
};

}  // namespace mysql_handler
#endif
