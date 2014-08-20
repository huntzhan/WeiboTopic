// ============================================================================
//
//       Filename:  test_mysql.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/19/2014 15:29:42
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"

#include "database/mysql_handler.h"

TEST(test_mysql_connector, test_conn_setup) {
  mysql_handler::SimpleConnectionSetup conn_setup;
  auto conn = conn_setup.RetrieveConnection();
  EXPECT_NE(nullptr, conn.get());
}

TEST(test_mysql_connector, test_conn_setup_with_arguments) {
  mysql_handler::SimpleConnectionSetup conn_setup(
    "tcp://127.0.0.1:3306",
    "root",
    "123456",
    "testcase");
  auto conn = conn_setup.RetrieveConnection();
  EXPECT_NE(nullptr, conn.get());
}

TEST(test_mysql_connector, test_operator) {
  mysql_handler::SimpleOperator op;
  op.Init();
}
