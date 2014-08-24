// ============================================================================
//
//       Filename:  item.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/22/2014 19:07:44
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include "data_mining/item_related.h"

#include <cstddef>
#include <iterator>

#include "utils/calculator.h"


using std::size_t;
using std::distance;
using utils::Cosine;


namespace data_mining {


Features AdapterForBitset::GetFeatures() const {
  return features_;
}


int AdapterForBitset::GetID() const {
  return id_;
}


double ItemWithCosineDistance::Similarity(
    const SharedPtrItem &other) const {
  auto other_features = other->features();
  return Cosine::Evaluate(features(), other_features);
}


ItemSetWithCosineDistance::ItemSetWithCosineDistance(
    const SharedPtrItem &item) {
  set_features(item->features());
  add_item(item);
}


double ItemSetWithCosineDistance::Similarity(
    const SharedPtrItemSet &other) const {
  auto other_features = other->features();
  return Cosine::Evaluate(features(), other_features);
}


void ItemSetWithCosineDistance::UpdateMeanFeatures() {
  auto contained_items = items();
  auto dimension = features().size();

  Features sum_of_features(dimension, 0.0);
  // sum up.
  for (const SharedPtrItem &item : contained_items) {
    // get feature of item.
    auto item_features = item->features();
    // sum up features.
    for (auto iter = item_features.cbegin();
         iter != item_features.cend(); ++iter) {
      auto index = distance(item_features.cbegin(), iter);
      sum_of_features[index] += *iter;
    }
  }
  // calculate mean.
  for (auto &feature_value : sum_of_features) {
    feature_value /= contained_items.size();
  }
  set_features(sum_of_features);
}


void ItemSetWithCosineDistance::Merge(
    const SharedPtrItemSet &other) {
  // merge itmes.
  auto other_items = other->items();
  for (const SharedPtrItem &item : other_items) {
    add_item(item);
  }
  // update mean features;
  UpdateMeanFeatures();
}


}  // namespace data_mining
