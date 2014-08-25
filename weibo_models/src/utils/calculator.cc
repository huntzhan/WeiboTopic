// ============================================================================
//
//       Filename:  calculator.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/24/2014 21:31:06
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include "utils/calculator.h"
#include "data_mining/interface.h"


using data_mining::ListSharedPtrItemSet;


namespace utils {


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


}  // namespace utils
