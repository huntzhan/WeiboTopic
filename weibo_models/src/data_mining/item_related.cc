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
    const ItemInterface &other) const {
  auto other_features = other.GetFeatures();
  return Cosine::Evaluate(features_, other_features);
}


Features ItemSetWithCosineDistance::mean_features() const {
  return mean_features_;
}


ItemSetWithCosineDistance::ItemSetWithCosineDistance(
    const ItemWithCosineDistance &item) {
  set_features(item.GetFeatures());
  items_.push_back(item);
}


double ItemSetWithCosineDistance::Similarity(
    const ItemSetInterface &other) {
  auto other_features = other.features();
  auto this_features = features();
  return Cosine::Evaluate(this_features, other_features);
}


std::vector<ItemWithCosineDistance>
ItemSetWithCosineDistance::items() const {
  return items_;
}


void ItemSetWithCosineDistance::UpdateMeanFeatures() {
  auto set_of_features = features();
  auto dimension = set_of_features[0].size();

  Features sum_of_features(dimension, 0.0);
  // sum up.
  for (const auto &features: set_of_features) {
    for (auto iter = features.cbegin(); iter != features.cend(); ++iter) {
      auto index = distance(features.cbegin(), iter);
      sum_of_features[index] += *iter;
    }
  }
  // calculate mean.
  for (auto &feature_value : sum_of_features) {
    feature_value /= set_of_features.size();
  }
  set_features(sum_of_features);
}


void ItemSetWithCosineDistance::Merge(
    const ItemSetInterface &other) {
  // merge itmes.
  auto other_items = other.items();
  for (const auto &item : other_items) {
    items_.push_back(item);
  }
  // update mean features;
  UpdateMeanFeatures();
}


}  // namespace data_mining
