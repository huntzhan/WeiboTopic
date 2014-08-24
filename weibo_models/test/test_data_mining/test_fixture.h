// ============================================================================
//
//       Filename:  test_fixture.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/24/2014 14:54:01
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <vector>

#include "gtest/gtest.h"

#include "data_mining/interface.h"
#include "data_mining/item_related.h"
#include "data_mining/cluster_related.h"


using std::vector;

using data_mining::ItemWithCosineDistance;
using data_mining::ItemSetWithCosineDistance;

using data_mining::SharedPtrItem;
using data_mining::SharedPtrItemSet;
using data_mining::ListSharedPtrItemSet;
using data_mining::AuxiliaryFunc;

using data_mining::Features;

#ifndef TEST_DATA_MINING_TEST_FIXTURE_H_
#define TEST_DATA_MINING_TEST_FIXTURE_H_
class TestItem : public ::testing::Test {
 public:
  SharedPtrItem GetItem(Features features) {
    SharedPtrItem item(new ItemWithCosineDistance);
    item->set_features(features);
    return item;
  }
};


class TestItemSet : public ::testing::Test {
 public:
  SharedPtrItemSet GetItemSet(const int &id,
                              vector<double> features) {
    SharedPtrItem item(new ItemWithCosineDistance);
    item->set_id(id);
    item->set_features(features);

    SharedPtrItemSet item_set(new ItemSetWithCosineDistance(item));
    item_set->set_id(id);
    return item_set;
  }

  ListSharedPtrItemSet GetTestListSharedPtrItemSet(vector<int> indexes) {
    ListSharedPtrItemSet item_sets;
    for (const int &index : indexes) {
      auto new_item_set = GetItemSet(index, {});
      AuxiliaryFunc::InsertItemSet(&item_sets, new_item_set);
    }
    return item_sets;
  }
};
#endif
