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
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <string>
#include <memory>
#include <vector>

#include "mysql_driver.h"
#include "cppconn/driver.h"
#include "cppconn/connection.h"
#include "db/model.h"

#ifndef _DATABASE_MYSQL_HANDLER_H_
#define _DATABASE_MYSQL_HANDLER_H_
namespace mysql_handler {

using SharedConn = std::shared_ptr<sql::Connection>;

// @brief: class to target table.
class DatabaseLocation {
 public:
  std::string url_;
  std::string username_;
  std::string password_;
  std::string database_;
};


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
  // constructor that use (url, username, password, database) to target db.
  SimpleConnectionSetup(const DatabaseLocation &tl)
      : db_location_(tl) {/* empty */};
  // implemented interface.
  SharedConn RetrieveConnection() const override;

 private:
  const DatabaseLocation &db_location_;
};


// @brief: Use BasicConnectionSetup and its derived class to open connection.
// Then use such connection to carried out operations.
class BasicHandler {
 public:
  // @brief: setup connection.
  virtual void Init() const final;  // forbids override.
  virtual SharedConn current_conn() const final;
  virtual SharedConn set_current_conn() const = 0;

 private:
  mutable SharedConn current_conn_ = nullptr;
};


// @brief: Handler that operation on single table.
class SimpleHandler : public BasicHandler {
 public:
  SimpleHandler(const std::string &db_name, const std::string &table_name);
  SharedConn set_current_conn() const override;

  std::string table_name() const;

 private:
  // default values of (url, username, password, database).
  DatabaseLocation db_location_;
  // name of table being operated.
  const std::string table_name_;
};


class SpamHandler : public SimpleHandler {
 public:
  SpamHandler(const std::string &db_name, const std::string &table_name)
      : SimpleHandler(db_name, table_name) {/* empty */}
  int AddSpams(std::vector<unsigned int> &spams);
  bool QuerySpamSimhash(std::vector<unsigned int> &spam);
};

class FilteredBlogHandler : public SimpleHandler {
 public:
  FilteredBlogHandler(const std::string &db_name, const std::string &table_name)
      : SimpleHandler(db_name, table_name) {/* empty */}
  unsigned int AddFilteredBlogs(std::vector<ParsedBlog> &blogs);
};

}  // namespace mysql_handler
#endif
