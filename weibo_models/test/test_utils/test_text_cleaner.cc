// ============================================================================
//
//       Filename:  test_text_cleaner.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/21/2014 18:19:04
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"

#include <string>
#include "utils/text_cleaner.h"

using std::string;
using utils::TextCleaner;

TEST(test_utils, test_text_cleaner) {
  TextCleaner cleaner;
  string text = "#产妇丧命医护失踪# 事件一波三折，你怎么http://t.cn/RPWBXri看？ "
                "相信医护人员，责任在家属 详情:http://t.cn/RPWBXri";
  string result = cleaner.Clean(text);
  EXPECT_EQ("产妇丧命医护失踪 事件一波三折，你怎么看？ "
            "相信医护人员，责任在家属 详情", result);
}
