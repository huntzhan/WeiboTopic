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


namespace utils {


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
