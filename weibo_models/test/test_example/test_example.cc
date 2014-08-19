// ============================================================================
//
//       Filename:  test_example.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/17/2014 22:50:33
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (zhx), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"
#include "example/simple_calculator.h"

using example::SimpleCalculator;

TEST(test_example, test_func) {
  SimpleCalculator handle;
  EXPECT_EQ(42, handle.SimpleFunc());
}
