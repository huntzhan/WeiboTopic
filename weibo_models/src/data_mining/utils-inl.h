// ============================================================================
//
//       Filename:  utils-inl.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/25/2014 14:57:41
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
namespace data_mining {


template <typename Iterable>
Features MeanFeatures::Calculate(
    const Iterable &set_with_features_interface,
    const int &dimension) {

  Features mean_features(dimension, 0.0);
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
double CatergoryUtilityEvaluator::CalculateSumOfSquares(
    const Iterable &values) {
  double sum_of_squares = 0.0;
  for (const double &value : values) {
    sum_of_squares += value * value;
  }
  return sum_of_squares;
}


}  // namespace data_mining
