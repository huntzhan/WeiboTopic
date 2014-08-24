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

TEST(test_mysql_connector, test_topic_Handler) {
  mysql_handler::TopicHandler handler("testcase", "SingleTopic");
  handler.Init();

  auto results = handler.GetMessages();
  EXPECT_GT(results.size(), 10);
}
