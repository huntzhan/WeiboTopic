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
#include "data_mining/interface.h"


using std::make_pair;
using std::multimap;


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


VecSharedPtrItem MaxSimilarityItemInItemSet::TopK(
    const SharedPtrItemSet &item_set,
    const int &number) {
  auto features_of_set = item_set->features();

  multimap<double, SharedPtrItem> evaluation_item_mapping;

  for (const SharedPtrItem &item : item_set->items()) {
    // calculate evaluation of current features.
    auto features_of_item = item->features();
    double current_evaluation = Cosine::Evaluate(
        features_of_set, features_of_item);

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
