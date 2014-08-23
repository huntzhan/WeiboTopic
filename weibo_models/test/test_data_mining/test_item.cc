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
using data_mining::AdapterForBitset;

TEST(test_dm, test_adapter) {
  BitsetFeatures<10> message(~0);  // set all bits to be 1.
  AdapterForBitset adapter(message, 3);

  EXPECT_EQ(3, adapter.GetID());
  auto features = adapter.GetFeatures();
  for (const auto &value : features) {
    EXPECT_EQ(1.0, value);
  }
}
