// ============================================================================
//
//       Filename:  calculator.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/22/2014 17:38:57
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <cstddef>
#include <cmath>

#include "data_mining/interface.h"


namespace utils {


template <typename Iterable>
data_mining::Features MeanFeatures::Calculate(
    const Iterable &set_with_features_interface,
    const int &dimension) {
  data_mining::Features mean_features(dimension, 0.0);
  // sum up features.
  for (const auto &target : set_with_features_interface) {
    // target should be a pointer to an object with features() member function.
    auto features = target->features();
    for (auto feature_iter = features.cbegin();
         feature_iter != features.cend(); ++feature_iter) {
      auto index = distance(features.cbegin(), feature_iter);
      mean_features[index] += *feature_iter;
    }
  }
  // calculate mean.
  for (auto &feature : mean_features) {
    feature /= set_with_features_interface.size();
  }
  return mean_features;
}


template <typename Iterable>
double Cosine::Evaluate(const Iterable &x, const Iterable &y) {
  if (x.size() != y.size()) {
    return 0.0;
  }
  double top = 0;
  double bottom_x = 0, bottom_y = 0;
  for (std::size_t index = 0; index != x.size(); ++index) {
    top += x[index] * y[index];
    bottom_x += x[index] * x[index];
    bottom_y += y[index] * y[index];
  }
  // force zero for empty vector.
  if (!bottom_x || !bottom_y) {
    return 0.0;
  }
  return top / (std::sqrt(bottom_x) * std::sqrt(bottom_y));
}


template <typename Iterable>
double CatergoryUtilityEvaluator::CalculateSumOfSquares(
    const Iterable &values) {
  double sum_of_squares = 0.0;
  for (const double &value : values) {
    sum_of_squares += value * value;
  }
  return sum_of_squares;
}


}  // namespace utils
