// ============================================================================
//
//       Filename:  utils.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/25/2014 15:21:02
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include "data_mining/utils.h"

#include <map>
#include <utility>
#include <functional>
#include "data_mining/interface.h"


using std::make_pair;
using std::multimap;
using std::greater_equal;


namespace data_mining {


void CatergoryUtilityEvaluator::Init(const int &size,
                                     const double &sum_of_squares) {
  size_ = size;
  sum_of_squares_ = sum_of_squares;
}


double CatergoryUtilityEvaluator::Evaluate(
    const ListSharedPtrItemSet &item_sets) const {
  // CU value.
  double catergory_utility = 0.0;
  for (const auto &item_set : item_sets) {
    auto features = item_set->features();
    auto items = item_set->items();
    // it's important to cast to double.
    double weight = items.size() / static_cast<double>(size_);
    // sum of suqares of features of currrent item set.
    double sq_of_item_set = CalculateSumOfSquares(features);
    catergory_utility += weight * (sq_of_item_set - sum_of_squares_);
  }
  catergory_utility /= item_sets.size();
  return catergory_utility;
}


VecSharedPtrItem MaxEvaluationItemInItemSet::TopK(
    const ListSharedPtrItemSet &item_sets,
    const SharedPtrItemSet &item_set,
    const int &size) {
  // evaluation based on:
  // 1. similarity between item and item set it belongs to.
  // 2. dissimilarity between item and other item sets.
  auto features_of_set = item_set->features();
  multimap<double, SharedPtrItem, greater_equal<double>>
      evaluation_item_mapping;

  // calculate total items.
  int number_of_items = 0;
  for (const auto &item_set_to_be_count : item_sets) {
    number_of_items += item_set_to_be_count->items().size();
  }

  for (const SharedPtrItem &item : item_set->items()) {
    auto features_of_item = item->features();
    // similarity.
    double similarity = Cosine::Evaluate(
        features_of_set, features_of_item);

    // dissimilarity.
    double dissimilarity = 0.0;
    for (const auto &other_item_set : item_sets) {
      if (other_item_set == item_set) { continue; }
      auto other_features = other_item_set->features();
      auto other_weight =
          other_item_set->items() / static_cast<double>(number_of_items);
      // calculate dissimilarity based on weight and cosine distance.
      dissimilarity += other_weight * Cosine::Evaluate(
          other_features, features_of_item);
    }
    // generate evaluation.
    double current_evaluation = similarity - dissimilarity;
    evaluation_item_mapping.insert(
        make_pair(current_evaluation, item));
  }
  // select top k.
  VecSharedPtrItem top_k_items;
  int counter = 0;
  for (const auto &mapping : evaluation_item_mapping) {
    top_k_items.push_back(mapping.second);
    ++counter;
    if (counter == number) {
      break;
    }
  }
  return top_k_items;
}


}  // namespace data_mining
