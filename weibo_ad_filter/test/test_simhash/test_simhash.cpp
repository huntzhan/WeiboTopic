
// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:

#include <limits.h>
#include "simhash/simhash.h"
#include "gtest/gtest.h"
#include <iostream>
using std::cout;
using std::endl;

// Step 2. Use the TEST macro to define your tests.
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.

TEST(TestSimhash, TestHammingDist) {
  SimHash sim;
  unsigned int value = 0;
  cout<<"###Hamming Dist: "<< endl;
  cout<<"###Origin: "<< value<< endl;
  cout<<"###Dist 1: " << endl;
  vector<unsigned int> values;
  sim.HammingValuesWithinDistance(value, 1, values);
  for(const auto &v : values){
    cout<<v<<endl;
  }
  values.clear();
  cout<<"###Dist 2: " << endl;
  sim.HammingValuesWithinDistance(value, 2, values);
  for(const auto &v : values){
    cout<<v<<endl;
  }
}

// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
