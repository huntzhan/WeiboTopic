// ============================================================================
//
//       Filename:  reduce_dimension.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/21/2014 14:38:14
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#ifndef UTILS_DIMENSION_REDUCER_H_
#define UTILS_DIMENSION_REDUCER_H_

#include <string>
#include <vector>
#include <bitset>

namespace utils {

// auxiliary types.
using VecStr = std::vector<std::string>;

template <int feature_size>
using VecBitset = std::vector<std::bitset<feature_size>>;


// @brief: Extract keywords of a set of messages and vectorize messages.
template <int T>
class DimensionReducerInterface {
 public:
  // interfaces.
  virtual void Process(const VecStr &messages) = 0;
  virtual VecStr GetKeywords() = 0;
  virtual VecBitset<T> GetVectorizedMessages() = 0;
};


template <int T>
class TFIDFDimensionReducer : public DimensionReducerInterface<T> {
 public:
  void Process(const VecStr &messages) override;
  VecStr GetKeywords() override;
  VecBitset<T> GetVectorizedMessages() override;
 
 private:
  VecStr keywords_;
  VecBitset<T> vectorized_messages_;
};

}  // namespace utils

#include "utils/dimension_reducer.cc"

#endif
