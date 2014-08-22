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

#include <iostream>
#include <vector>
#include "gtest/gtest.h"

#include "database/mysql_handler.h"
using std::cout;
using std::endl;
using std::vector;

TEST(test_mysql_connector, test_conn_setup_with_arguments) {
  // set db.
  mysql_handler::DatabaseLocation db_location;
  db_location.url_ = "tcp://127.0.0.1:3306";
  db_location.username_ = "root";
  db_location.password_ = "123456";
  db_location.database_ = "testcase";

  mysql_handler::SimpleConnectionSetup conn_setup(db_location);

  auto conn = conn_setup.RetrieveConnection();
  EXPECT_NE(nullptr, conn.get());
}

TEST(test_mysql_connector, test_Handler) {
  mysql_handler::SimpleHandler handler("testcase", "SingleTopic");
  handler.Init();
}

TEST(test_mysql_connector, test_spam_Handler) {
  mysql_handler::SpamHandler handler("simhash", "spam");
  handler.Init();

  vector<unsigned int> v;
  bool result;

  v.push_back(1);
  result = handler.QuerySpamSimhash(v);
  v.clear();

  for(int i= 2; i<1000; i++)
    v.push_back(i);
  result = handler.QuerySpamSimhash(v);
  EXPECT_FALSE(result);

  /// don't play with insertion ordinarily
  // int AddSpams(vector<unsigned int> spams);
  // int count = handler.AddSpams(v);
  // EXPECT_EQ(998, count);
}
