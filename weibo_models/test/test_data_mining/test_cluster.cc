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

#include <iostream>
using std::cout;
using std::endl;

using std::vector;
using std::initializer_list;
using std::random_shuffle;

using data_mining::ItemWithCosineDistance;
using data_mining::ItemSetInterface;
using data_mining::ItemSetWithCosineDistance;

using data_mining::SharedPtrItem;
using data_mining::SharedPtrItemSet;
using data_mining::ListSharedPtrItemSet;
using data_mining::AuxiliaryFunc;


// class TestItemSet : public ::testing::Test {
//  public:
//   SharedPtrItemSet GetItemSet(const int &id,
//                               initializer_list<double> features) {
//     SharedPtrItem item(new ItemWithCosineDistance);
//     item->set_id(id);
//     item->set_features(features);
// 
//     SharedPtrItemSet item_set(new ItemSetWithCosineDistance(item));
//     ItemSetWithCosineDistance item_set(item);
//     item_set.set_id(id);
//     return item_set;
//   }
// 
//   ListRefItemSets GetTestListRefItemSets(vector<int> indexes) {
//     ListRefItemSets item_sets;
//     for (const int &index : indexes) {
//       auto new_item_set = GetItemSet(index, {});
//       AuxiliaryFunc::InsertItemSetToListRefItemSets(
//           &item_sets, new_item_set);
//       // debug.
//       for (const ItemSetInterface &item_set : item_sets) {
//         cout << item_set.id() << " ";
//       }
//       cout << endl;
//     }
//     return item_sets;
//   }
// };


TEST_F(TestItemSet, TestMakeItemSetRefPair) {

  auto item_set_0 = GetItemSet(0, {});
  auto item_set_1 = GetItemSet(1, {});
  auto ref_pair = AuxiliaryFunc::MakeItemSetRefPair(
      item_set_1, item_set_0);
  
  EXPECT_EQ(0, ref_pair.first.get().id());
  EXPECT_EQ(1, ref_pair.second.get().id());
}


TEST_F(TestItemSet, TestInsertAndSearchAndDelete) {
  vector<int> indexes = {2, 1, 4, 3, 0, 5};
  vector<int> expect_result = {0, 1, 2, 3, 4, 5};

  auto test_item_sets = GetTestListRefItemSets(indexes);

  // should be sorted.
  vector<int> run_result;
  for (const ItemSetInterface &item_set : test_item_sets) {
    run_result.push_back(item_set.id());
  }
  EXPECT_EQ(expect_result, run_result);

  // search.
  auto target_item_set = GetItemSet(3, {});
  auto iter = AuxiliaryFunc::BinarySearchListRefItemSets(
      &test_item_sets, target_item_set);
  EXPECT_EQ(3, iter->get().id());

  // delete.
  AuxiliaryFunc::RemoveItemSetRefFromListRefItemSets(
      &test_item_sets, iter);
  auto pre_item_set = GetItemSet(2, {});
  auto pre_iter = AuxiliaryFunc::BinarySearchListRefItemSets(
      &test_item_sets, pre_item_set);
  ++pre_iter;
  EXPECT_EQ(4, pre_iter->get().id());
}
