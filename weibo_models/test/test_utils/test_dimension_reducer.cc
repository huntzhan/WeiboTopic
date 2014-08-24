// ============================================================================
//
//       Filename:  test_dimension_reducer.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/21/2014 15:49:19
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"

#include "utils/dimension_reducer.h"
#include "database/mysql_handler.h"

#include <algorithm>
#include <string>

using utils::TFIDFDimensionReducer;
using std::find;

TEST(test_utils, test_tfidfdimensionreducer) {
  mysql_handler::TopicHandler handler("testcase", "SingleTopic");
  handler.Init();
  auto results = handler.GetMessages();

  TFIDFDimensionReducer<20> reducer;
  reducer.Process(results);
  auto keywords = reducer.GetKeywords();
  auto messages = reducer.GetVectorizedMessages();

  EXPECT_NE(keywords.cend(), find(keywords.cbegin(), keywords.cend(), "小米"));
  bool flag = false;
  for (const auto &message : messages) {
    if (message.any()) {
      flag = true;
    }
  }
  EXPECT_TRUE(flag);
}
