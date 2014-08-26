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


SharedPtrItem MaxSimilarityItemInItemSet::Find(
    const SharedPtrItemSet &item_set) {
  auto features_of_set = item_set->features();

  double max_similarity = 0.0;
  SharedPtrItem target_item;
  for (const SharedPtrItem &item : item_set->items()) {
    auto features_of_item = item->features();
    double current_similarity = Cosine::Evaluate(
        features_of_set, features_of_item);
    if (current_similarity > max_similarity) {
      max_similarity = current_similarity;
      target_item = item;
    }
  }
  return target_item;
}


}  // namespace data_mining
