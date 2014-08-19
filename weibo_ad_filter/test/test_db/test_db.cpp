
// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:

#include <limits.h>
#include "gtest/gtest.h"
#include "DB/connection_pool.h"
#include "DB/DBoperation.h"
#include "DB/DBpool.h"

// Step 2. Use the TEST macro to define your tests.
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.

#define SQL_ADDR "192.168.1.108"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"

TEST(TestDB, TestConn) {
  DBpool insert;
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root",
     "123456", 50);
  insert.DBinit("use test", connpool);
}

TEST(TestDB, TestDBPool) {
  DBoperation query;
  query.DBinit(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
}
// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
