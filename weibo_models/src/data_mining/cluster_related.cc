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

#include <vector>
#include <utility>
#include <algorithm>

#include "data_mining/cluster_related.h"


using std::vector;
using std::swap;
using std::upper_bound;
using std::lower_bound;


namespace data_mining {


ItemSetRefPair AuxiliaryFunc::MakeItemSetRefPair(
    const ItemSetRef &item_a, const ItemSetRef &item_b) {
  auto id_a = item_a.get().id();
  auto id_b = item_b.get().id();
  if (id_a > id_b) { 
    return ItemSetRefPair(item_b, item_a);
  } else {
    return ItemSetRefPair(item_a, item_b);
  }
}



ListRefItemSets::iterator
AuxiliaryFunc::BinarySearchListRefItemSets(
    ListRefItemSets *item_sets, const ItemSetRef &item_set) {
  return lower_bound(item_sets->begin(), item_sets->end(),
                     item_set, item_set_compare);
}


void AuxiliaryFunc::InsertItemSetToListRefItemSets(
    ListRefItemSets *item_sets, const ItemSetRef &item_set) {
  // find insert point.
  auto insert_point =
      std::upper_bound(item_sets->begin(), item_sets->end(),
                       item_set, item_set_compare);
  item_sets->insert(insert_point, item_set);
}


void HierarchyClustering::AddItem(const AdapterInterface &adapter) {
  auto id = adapter.GetID();
  auto features = adapter.GetFeatures();
  // init item.
  ItemWithCosineDistance item;
  item.set_features(features);
  item.set_id(id);
  // init item_set which contains only the current item.
  ItemSetWithCosineDistance item_set(item);
  // keep it.
  AuxiliaryFunc::InsertItemSetToListRefItemSets(&item_sets_, item_set);
}


void HierarchyClustering::Prepare() {

}


void HierarchyClustering::CarryOutCluster() {

}


vector<ClusterResult> HierarchyClustering::GetClusterResults() {
  return {};
}


}  // namespace data_mining
