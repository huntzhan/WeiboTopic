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

#include "gtest/gtest.h"

#include <vector>
#include <initializer_list>

#include "data_mining/interface.h"
#include "data_mining/item_related.h"
#include "data_mining/cluster_related.h"

// #include <iostream>
// using std::cout;
// using std::endl;

using std::vector;
using std::initializer_list;
using std::random_shuffle;

using data_mining::ItemWithCosineDistance;
using data_mining::ItemSetWithCosineDistance;

using data_mining::ItemSetRefPair;
using data_mining::ListRefItemSets;
using data_mining::AuxiliaryFunc;


class TestItemSet : public ::testing::Test {
 public:
  ItemWithCosineDistance GetItemSet(const int &id,
                                    initializer_list<double> features) {
    ItemWithCosineDistance item;
    item.set_id(id);
    item.set_features(features);

    ItemSetWithCosineDistance item_set(item);
    item_set.set_id(id);
    return item_set;
  }

  ListRefItemSets GetTestListRefItemSets(vector<int> indexes) {
    ListRefItemSets item_sets;
    for (const int &index : indexes) {
      auto new_item_set = GetItemSet(index, {});
      AuxiliaryFunc::InsertItemSetToListRefItemSets(
          &item_sets, new_item_set);
    }
    return item_sets;
  }
};


TEST_F(TestItemSet, TestMakeItemSetRefPair) {

  auto item_set_0 = GetItemSet(0, {});
  auto item_set_1 = GetItemSet(1, {});
  auto ref_pair = AuxiliaryFunc::MakeItemSetRefPair(
      item_set_1, item_set_0);
  
  EXPECT_EQ(0, ref_pair.first.get().id());
  EXPECT_EQ(1, ref_pair.second.get().id());
}


TEST_F(TestItemSet, TestInsertAndSearchAndDelete) {
  vector<int> indexes = {0, 1, 2, 3, 4, 5};
  vector<int> expect_result = indexs;
  random_shuffle(indexes.begin(), indexes.end());

  auto test_item_sets = GetTestListRefItemSets(indexes);

  // should be sorted.
  vector<int> run_result;
  for (const ItemSetInterface &item_set : item_sets) {
    run_result.push_back(item_set.id());
  }
  EXPECT_EQ(expect_result, run_result);

  // search.
  auto target_item_set = GetItemSet(3, {});
  auto iter = AuxiliaryFunc::BinarySearchListRefItemSets(
      &item_sets, target_item_set);
  EXPECT_EQ(3, iter->get().id());

  // delete.
  AuxiliaryFunc::RemoveItemSetRefFromListRefItemSets(
      &item_sets, iter);
  auto pre_item_set = GetItemSet(2, {});
  auto pre_iter = AuxiliaryFunc::BinarySearchListRefItemSets(
      &item_sets, pre_item_set);
  auto post_iter = pre_iter + 1;
  EXPECT_EQ(4, post_iter->get().id());
}
