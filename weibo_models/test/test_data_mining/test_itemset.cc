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

#include "data_mining/item_related.h"


using data_mining::ItemWithCosineDistance;
using data_mining::ItemSetWithCosineDistance;


TEST(test_dm, test_itemset_setup) {
  ItemWithCosineDistance item;
  item.set_features({0.5, 0.4});

  ItemSetWithCosineDistance itemset(item);
  std::vector<double> result = {0.5, 0.4};
  EXPECT_EQ(result, itemset.features());
}


TEST(test_dm, test_itemset_merge) {
  ItemWithCosineDistance item_a, item_b;
  item_a.set_features({1.0, 0.0});
  item_b.set_features({0.0, 1.0});

  ItemSetWithCosineDistance itemset_a(item_a);
  ItemSetWithCosineDistance itemset_b(item_b);
  itemset_a.Merge(itemset_b);

  std::vector<double> result = {0.5, 0.5};
  EXPECT_EQ(result, itemset_a.features());
}


TEST(test_dm, test_itemset_similarity) {
  ItemWithCosineDistance item_a, item_b;
  item_a.set_features({0.5, 0.4});
  item_b.set_features({0.0, 0.3});

  ItemSetWithCosineDistance itemset_a(item_a);
  ItemSetWithCosineDistance itemset_b(item_b);

  EXPECT_LT(0.624, itemset_a.Similarity(itemset_b));
  EXPECT_GT(0.625, itemset_a.Similarity(itemset_b));
}
