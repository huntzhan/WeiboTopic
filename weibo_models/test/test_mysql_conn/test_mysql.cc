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
//         Author:  Zhan Haoxun (zhx), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"

#include "mysql_driver.h"


TEST(test_mysql_connector, test_connect) {
  auto driver = sql::mysql::get_mysql_driver_instance();
  auto con = driver->connect("tcp://127.0.0.1:3306", "root", "123456");
  delete con;
}
