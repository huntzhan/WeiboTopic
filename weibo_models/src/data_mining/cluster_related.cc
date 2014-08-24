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
#include <memory>

#include "data_mining/cluster_related.h"


using std::vector;
using std::pair;
using std::swap;
using std::upper_bound;
using std::lower_bound;
using std::max_element;


namespace data_mining {


SharedPtrItemSetPair AuxiliaryFunc::MakeItemSetPair(
    const SharedPtrItemSet &set_a, const SharedPtrItemSet &set_b) {
  auto id_a = set_a->id();
  auto id_b = set_b->id();
  if (id_a > id_b) { 
    return SharedPtrItemSetPair(set_b, set_a);
  } else {
    return SharedPtrItemSetPair(set_a, set_b);
  }
}



ListSharedPtrItemSet::iterator
AuxiliaryFunc::SearchItemSet(ListSharedPtrItemSet *item_sets,
                             const SharedPtrItemSet &item_set) {
  return lower_bound(item_sets->begin(), item_sets->end(),
                     item_set, item_set_compare);
}


void AuxiliaryFunc::RemoveItemSet(
    ListSharedPtrItemSet *item_sets,
    const ListSharedPtrItemSet::iterator &iter) {
  item_sets->erase(iter);
}


void AuxiliaryFunc::InsertItemSet(ListSharedPtrItemSet *item_sets,
                                  const SharedPtrItemSet &item_set) {
  // find insert point.
  auto insert_point =
      std::upper_bound(item_sets->begin(), item_sets->end(),
                       item_set, item_set_compare);
  item_sets->insert(insert_point, item_set);
}


SharedPtrItemSetPair
AuxiliaryFunc::FindMaxSimilarity(SimilarityMap *similarity_map) {
  using IterType = pair<SharedPtrItemSetPair, double>;
  auto compare_value = [](const IterType &x, const IterType &y) {
    return x.second < y.second;
  };

  auto max_similarity_iter = max_element(
      similarity_map->cbegin(), similarity_map->cend(),
      compare_value);
  return max_similarity_iter->first;
}


void HierarchyClustering::AddItem(const AdapterInterface &adapter) {
  auto id = adapter.GetID();
  auto features = adapter.GetFeatures();
  // init item.
  SharedPtrItem item(new ItemWithCosineDistance);
  item->set_id(id);
  item->set_features(features);

  // init item_set which contains only the current item.
  SharedPtrItemSet item_set(new ItemSetWithCosineDistance(item));
  // id of item_set must be configured.
  item_set->set_id(item_sets_.size());
  // keep it.
  AuxiliaryFunc::InsertItemSet(&item_sets_, item_set);
}


void HierarchyClustering::Prepare() {
  // set up similarity_of_item_sets_;
  for (auto left_iter = item_sets_.cbegin();
       left_iter != item_sets_.cend(); ++left_iter) {

    auto right_iter = left_iter;
    ++right_iter;
    for (; right_iter != item_sets_.cend(); ++right_iter) {
      auto item_set_pair = AuxiliaryFunc::MakeItemSetPair(
          *left_iter, *right_iter);
      similarity_of_item_sets_[item_set_pair] =
        (*left_iter)->Similarity(*right_iter);
    }
  }
}


void HierarchyClustering::CarryOutCluster() {
  while (item_sets_.size() > 1) {
    auto max_pair = AuxiliaryFunc::FindMaxSimilarity(&similarity_of_item_sets_);
    auto left_set = max_pair.first;
    auto right_set = max_pair.second;

    // merge right_set to left_set;
    left_set->Merge(right_set);

    // remove right_set from item_sets_;
    auto pos = AuxiliaryFunc::SearchItemSet(&item_sets_, right_set);
    AuxiliaryFunc::RemoveItemSet(&item_sets_, pos);

    // remove (right_set, *) pair from similarity_of_item_sets_.
    for (const auto &item_set : item_sets_) {
      if (item_set->id() == right_set->id()) {
        continue;
      }
      auto right_set_related_pair = AuxiliaryFunc::MakeItemSetPair(
          item_set, right_set);
      similarity_of_item_sets_.erase(right_set_related_pair);
    }

    // update (left_set, *) pair of similarity_of_item_sets_;
    for (const auto &item_set : item_sets_) {
      if (item_set->id() == left_set->id()) {
        continue;
      }
      auto left_set_related_pair = AuxiliaryFunc::MakeItemSetPair(
          item_set, left_set);
      similarity_of_item_sets_[left_set_related_pair] =
          left_set->Similarity(item_set);
    }
  }
}


vector<ClusterResult> HierarchyClustering::GetClusterResults() {
  return {};
}


}  // namespace data_mining
