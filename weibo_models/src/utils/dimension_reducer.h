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
#include <cstddef>


namespace utils {

// auxiliary types.
using VecStr = std::vector<std::string>;

template <std::size_t dimension>
using BitsetFeatures = std::bitset<dimension>;

template <std::size_t dimension>
using VecBitset = std::vector<BitsetFeatures<dimension>>;


// @brief: Extract keywords of a set of messages and vectorize messages.
template <int dimension>
class DimensionReducerInterface {
 public:
  // interfaces.
  virtual void Process(const VecStr &messages) = 0;
  virtual VecStr GetKeywords() const = 0;
  virtual VecBitset<dimension> GetVectorizedMessages() const = 0;
};


template <int dimension>
class TFIDFDimensionReducer : public DimensionReducerInterface<dimension> {
 public:
  void Process(const VecStr &messages) override;
  VecStr GetKeywords() const override;
  VecBitset<dimension> GetVectorizedMessages() const override;
 
 private:
  VecStr keywords_;
  VecBitset<dimension> vectorized_messages_;
};

}  // namespace utils

#include "utils/dimension_reducer-inl.h"

#endif
