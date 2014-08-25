// ============================================================================
//
//       Filename:  calculator.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/22/2014 17:35:53
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#ifndef UTILS_CALCULATOR_H_
#define UTILS_CALCULATOR_H_
namespace utils {


class Cosine {
 public:
  template <typename Iterable>
  static double Evaluate(const Iterable &x, const Iterable &y);
};


class CatergoryUtilityEvaluator {
 public:
  void Init(const int &size, const double &sum_of_squares);
  double Evaluate(const data_mining::ListSharedPtrItemSet &item_sets) const;

  template <typename Iterable>
  static double CalculateSumOfSquares(const Iterable &values);

 private:
  int size_;
  double sum_of_squares_;
};


}  // namespace utils

#include "utils/calculator-inl.h"

#endif
