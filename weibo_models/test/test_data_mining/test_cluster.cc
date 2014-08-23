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

#include "data_mining/interface.h"
#include "data_mining/item_related.h"

using data_mining::ItemWithCosineDistance;
using data_mining::ItemSetWithCosineDistance;

using data_mining::ItemSetRefPair;
using data_mining::ListRefItemSets;
using data_mining::AuxiliaryFunc;


TEST(test_cluster, test_makeitemsetrefpair) {
  ItemWithCosineDistance item_0;
  ItemWithCosineDistance item_1;
  item_0.set_id(0);
  item_1.set_id(1);

  ItemSetWithCosineDistance item_set_0(item_0);
  ItemSetWithCosineDistance item_set_1(item_1);

  auto ref_pair = AuxiliaryFunc::MakeItemSetRefPair(
      item_set_1, item_set_0);
  
  EXPECT_EQ(0, ref_pair.first.get().id());
  EXPECT_EQ(1, ref_pair.second.get().id());
}
