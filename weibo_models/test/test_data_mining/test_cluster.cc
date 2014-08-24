// ============================================================================
//
//       Filename:  test_cluster.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/23/2014 18:52:27
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

#include "data_mining/cluster_related.h"


using std::vector;
using data_mining::AuxiliaryFunc;


TEST_F(TestItemSet, TestMakeItemSetRefPair) {

  auto item_set_0 = GetItemSet(0, {});
  auto item_set_1 = GetItemSet(1, {});
  auto item_set_pair = AuxiliaryFunc::MakeItemSetPair(
      item_set_1, item_set_0);
  
  EXPECT_EQ(0, item_set_pair.first->id());
  EXPECT_EQ(1, item_set_pair.second->id());
}


TEST_F(TestItemSet, TestInsertAndSearchAndDelete) {
  vector<int> indexes = {2, 1, 4, 3, 0, 5};
  vector<int> expect_result = {0, 1, 2, 3, 4, 5};

  auto test_item_sets = GetTestListSharedPtrItemSet(indexes);

  // should be sorted.
  vector<int> run_result;
  for (const auto &item_set : test_item_sets) {
    run_result.push_back(item_set->id());
  }
  EXPECT_EQ(expect_result, run_result);

  // search.
  auto target_item_set = GetItemSet(3, {});
  auto iter = AuxiliaryFunc::SearchItemSet(&test_item_sets,
                                           target_item_set);
  EXPECT_EQ(3, (*iter)->id());

  // delete.
  AuxiliaryFunc::RemoveItemSet(&test_item_sets, iter);
  auto pre_item_set = GetItemSet(2, {});
  auto pre_iter = AuxiliaryFunc::SearchItemSet(&test_item_sets,
                                               pre_item_set);
  ++pre_iter;
  EXPECT_EQ(4, (*pre_iter)->id());
}
