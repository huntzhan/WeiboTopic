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
using data_mining::ItemWithCosineDistance;
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


TEST(test_dm, test_item_similarity_1) {
  ItemWithCosineDistance item_a, item_b;
  item_a.set_features({0.5, 0.4});
  item_b.set_features({0.5, 0.4});
  EXPECT_EQ(1.0, item_a.Similarity(item_b));
}


TEST(test_dm, test_item_similarity_2) {
  ItemWithCosineDistance item_a, item_b;
  item_a.set_features({0.0, 1.0});
  item_b.set_features({1.0, 0.0});
  EXPECT_EQ(0.0, item_a.Similarity(item_b));
}


TEST(test_dm, test_item_similarity_3) {
  ItemWithCosineDistance item_a, item_b;
  item_a.set_features({0.5, 0.4});
  item_b.set_features({0.0, 0.3});
  EXPECT_LT(0.624, item_a.Similarity(item_b));
  EXPECT_GT(0.625, item_a.Similarity(item_b));
}
