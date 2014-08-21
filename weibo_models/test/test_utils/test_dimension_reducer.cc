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

#include <iostream>

using std::cout;
using std::endl;

using utils::TFIDFDimensionReducer;

TEST(test_utils, test_tfidfdimensionreducer) {
  mysql_handler::TopicHandler handler("testcase", "SingleTopic");
  handler.Init();
  auto results = handler.topic_for_test();

  TFIDFDimensionReducer<20> reducer;
  reducer.Process(results);
  auto keywords = reducer.GetKeywords();
  for (const auto &word : keywords) {
    cout << word << endl;
  }
}
