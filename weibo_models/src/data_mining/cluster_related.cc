// ============================================================================
//
//       Filename:  cluster_related.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/23/2014 17:54:15
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <utility>
#include <algorithm>

#include "data_mining/cluster_related.h"


using std::swap;
using std::upper_bound;
using std::binary_search;


namespace data_mining {


ItemSetRefPair AuxiliaryFunc::MakeItemSetRefPair(
    const ItemInterface &item_a, const ItemInterface &item_a) {
  auto id_a = item_a.id();
  auto id_b = item_b.id();
  if (id_a > id_b) { 
    return ItemSetInterface(item_b, item_a);
  } else {
    return ItemSetInterface(item_1, item_b);
  }
}



ListRefItemSets::iterator
AuxiliaryFunc::BinarySearchListRefItemSets(
    ListRefItemSets *item_sets, const ItemSetInterface &item_set) {
  return binary_search(item_sets_->begin(), item_sets_->end(),
                       item_set, item_set_compare);
}


void AuxiliaryFunc::InsertItemSetToListRefItemSets(
    ListRefItemSets *item_sets, const ItemSetInterface &item_set) {
  // find insert point.
  auto insert_point =
      std::upper_bound(item_sets_->begin(), item_sets_->end(),
                       item_set_compare);
  item_sets_.insert(insert_point, item_set);
}


void HierarchyClustering::AddItem(const AdapterInterface &adapter) {
  auto id = adapter.id();
  auto features = adapter.features();
  // init item.
  ItemWithCosineDistance item;
  item.set_features(features);
  item.set_id(id);
  // init item_set which contains only the current item.
  ItemSetWithCosineDistance item_set(item);
  // keep it.
  AuxiliaryFunc::InsertItemSetToListRefItemSets(&item_sets_, item_set);
}


}  // namespace data_mining
