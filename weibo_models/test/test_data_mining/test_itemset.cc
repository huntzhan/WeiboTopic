// ============================================================================
//
//       Filename:  test_itemset.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/23/2014 15:44:04
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <vector>

#include "gtest/gtest.h"
#include "test_data_mining/test_fixture.h"

#include "data_mining/item_related.h"


using std::vector;


TEST_F(TestItemSet, TestItemsetSetup) {
  vector<double> expected = {0.5, 0.4};
  auto item_set = GetItemSet(0, expected);
  EXPECT_EQ(0, item_set->id());
  EXPECT_EQ(expected, item_set->features());
}


TEST_F(TestItemSet, TestItemsetMerge) {
  auto item_set_a = GetItemSet(0, {1.0, 0.0});
  auto item_set_b = GetItemSet(1, {0.0, 1.0});
  item_set_a->Merge(item_set_b);

  vector<double> expected = {0.5, 0.5};
  EXPECT_EQ(expected, item_set_a->features());
}


TEST_F(TestItemSet, TestItemsetSimilarity) {
  auto item_set_a = GetItemSet(0, {0.5, 0.4});
  auto item_set_b = GetItemSet(1, {0.0, 0.3});

  EXPECT_LT(0.624, item_set_a->Similarity(item_set_b));
  EXPECT_GT(0.625, item_set_a->Similarity(item_set_b));
}
