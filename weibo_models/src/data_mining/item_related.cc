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

#include "data_mining/utils.h"
#include "utils/calculator.h"


using std::size_t;
using std::distance;
using utils::Cosine;
using data_mining::MeanFeatures;


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

  auto sum_of_features = MeanFeatures::Calculate(contained_items, dimension);
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
