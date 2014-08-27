// ============================================================================
//
//       Filename:  utils.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/25/2014 14:55:28
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include "data_mining/interface.h"


#ifndef DATA_MINING_UTILS_H_
#define DATA_MINING_UTILS_H_
namespace data_mining {


class Cosine {
 public:
  template <typename Iterable>
  static double Evaluate(const Iterable &x, const Iterable &y);
};


class MeanFeatures {
 public:
  template <typename Iterable>
  static data_mining::Features Calculate(
      const Iterable &set_with_features_interface,
      const int &dimension);
};


class CatergoryUtilityEvaluator {
 public:
  void Init(const int &size, const double &sum_of_squares);
  double Evaluate(const ListSharedPtrItemSet &item_sets) const;

  template <typename Iterable>
  static double CalculateSumOfSquares(const Iterable &values);

 private:
  int size_;
  double sum_of_squares_;
};


class MaxEvaluationItemInItemSet {
 public:
  static VecSharedPtrItem TopK(
      const ListSharedPtrItemSet &item_sets,
      const SharedPtrItemSet &item_set,
      const int &size);
};


}  // namespace data_mining


#include "data_mining/utils-inl.h"


#endif
