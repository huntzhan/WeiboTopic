
// This sample shows how to write a simple unit test for a function,
// using Google C++ testing framework.
//
// Writing a unit test using Google C++ testing framework is easy as 1-2-3:

#include <iostream>
#include <list>
#include <limits.h>
#include "gtest/gtest.h"
#include "allocator/allocator.h"
using std::cout;
using std::endl;
using std::list;

// Step 2. Use the TEST macro to define your tests.
// Google Test guarantees that each test you define is run exactly
// once, but it makes no guarantee on the order the tests are
// executed.  Therefore, you should write your tests in such a way
// that their results don't depend on their order.

TEST(TestAllocator, TestAllocator) {
  Allocator allo("Microblog1408215600");
  while (allo.HasNextTable()) {
    allo.NextTable();
    cout<< allo.GetRowsOfCurrentTable() << endl;
    std::list<Blog> blogs;
    while (allo.HasNextRow()) {
      unsigned count = allo.NextBlogs(5000, blogs);
      cout<< "get rows: " << count << endl;
    }
  }

}

// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
