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

TEST(test_mysql_connector, test_connect) {
  mysql_handler::ConnectionSetup connector;
  auto con = connector.RetrieveConnection();
  EXPECT_NE(nullptr, con.get());
}
