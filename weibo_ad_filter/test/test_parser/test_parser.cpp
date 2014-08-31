
// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:

#include <limits.h>
#include "gtest/gtest.h"
#include <string>
#include <vector>
#include "split/parser.h"
#include "split/Textspilt.h"
#include <iostream>
using std::cout;
using std::endl;

// Step 2. Use the TEST macro to define your tests.
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.

TEST(TestParser, TestParser) {
  TextSpilt::init_ICTCAL();
  Parser parser;

  // std::string rawtext("我真的很爱你");
  std::string rawtext("【为什么我们会失落呢？】生活中常有这种事情，来到跟前的往往轻易放过，远在天边的却又苦苦追求；占有它时总感平淡无味；失去的时候时方觉非常可惜。——没有坎坷的生活是幼稚的，缺少思考的生活是平庸的，失去追求的生活是空虚的，缺乏朝气的生活是贫乏的，毫无远见的生活是浅薄的。");
  // std::string rawtext("发表了博文《炒股入门：分时图是什么？怎么看大盘和个股分时图？》分时图是什么？分时图是指大盘和个股的动态实时（即时）分时走势图，其在实战研判中的地位极其重要，是即时把握多空力量转化即市场变化直接的根本所http://t.cn/RPjCUAo");
  // std::string rawtext("[音乐盒][音乐盒][音乐盒][音乐盒][音乐盒]（分享自 @新浪乐库）（分享自）（分享自）（分享自）（分享自）");
  std::vector<Word> words;
  parser.LexicalAnalysis(rawtext, words);
  cout<<"Words: "<<endl;
  for(auto i: words)
    cout<<i.word <<"("<<i.proper<<")"<<endl;
  cout<<"############"<<endl;
  EXPECT_TRUE(words.size() > 1);
}

// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
