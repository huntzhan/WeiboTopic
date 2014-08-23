// ============================================================================
//
//       Filename:  test_item.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/23/2014 12:47:30
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "gtest/gtest.h"

#include "data_mining/item_related.h"
#include "utils/dimension_reducer.h"


using utils::BitsetFeatures;
using utils::ItemWithCosineDistance;
using data_mining::AdapterForBitset;

TEST(test_dm, test_adapter) {
  BitsetFeatures<10> all_one_message(~0);  // set all bits to be 1.
  BitsetFeatures<10> all_zero_message(0);  // set all bits to be 0.
  AdapterForBitset all_one_adapter(all_one_message, 1);
  AdapterForBitset all_zero_adapter(all_zero_message, 0);

  EXPECT_EQ(1, all_one_adapter.GetID());
  EXPECT_EQ(0, all_zero_adapter.GetID());
  auto all_one_features = all_one_adapter.GetFeatures();
  auto all_zero_features = all_zero_adapter.GetFeatures();
  for (const auto &value : all_one_features) {
    EXPECT_EQ(1.0, value);
  }
  for (const auto &value : all_zero_features) {
    EXPECT_EQ(0.0, value);
  }
}


test(test_dm, test_item_similarity) {
  
}
