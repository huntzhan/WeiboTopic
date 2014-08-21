
// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.

#include <limits.h>
#include "gtest/gtest.h"
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "logger/logger.h"
#include "tactic/preprocessor.h"
#include "db/model.h"
using std::cout;
using std::endl;
using std::string;

class TestTactic : public testing::Test {
public:

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

};

TEST(TestTactic, TestZombieTactic) {
  Preprocessor pre;
  cout<<"#####"<<"Pre initialized"<<endl;
  Logger log("zombietactic.log");
  cout<<"#####"<<"Logger initialized"<<endl;

  while(log.HasNextBlog()){
    // test each blog
    Blog b = log.NextBlog();
    bool is_good_blog = pre.PerformTactic(b);
    if(! is_good_blog){
      log.AddRemovedBlog(b);
      // PrintBlog(b);
    }
  }

  log.ProduceFinalReport();
}

