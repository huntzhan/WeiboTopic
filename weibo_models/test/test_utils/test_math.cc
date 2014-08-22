// ============================================================================
//
//       Filename:  test_math.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/22/2014 18:51:02
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"

#include <vector>
#include "utils/calculator.h"

using std::vector;
using utils::Cosine;

TEST(test_utils, test_cosine_normal) {
  vector<double> x = {1.0, 2.0};
  vector<double> y = {2.0, 3.0};
  auto value = Cosine::Evaluate(x, y);
  EXPECT_LT(0.992, value);
  EXPECT_GT(0.993, value);
}

TEST(test_utils, test_cosine_empty) {
  vector<double> x = {0.0, 0.0};
  vector<double> y = {2.0, 3.0};
  auto value = Cosine::Evaluate(x, y);
  EXPECT_EQ(0.0, value);
}

TEST(test_utils, test_cosine_mismatch) {
  vector<double> x = {1.0, 0.0, 0.0};
  vector<double> y = {2.0, 3.0};
  auto value = Cosine::Evaluate(x, y);
  EXPECT_EQ(0.0, value);
}
