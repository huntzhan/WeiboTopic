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
#include "utils/calculator.h"


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


void ItemSetWithCosineDistance::UpdateMeanFeatures() const {

}


void ItemSetWithCosineDistance::Merge(
    const ItemSetInterface &other) {
  // merge itmes.
  auto other_items = other.items();
  for (const auto &item : items) {
    items_.push_back(item);
  }
  // update mean features;
  UpdateMeanFeatures();
}


}  // namespace data_mining
