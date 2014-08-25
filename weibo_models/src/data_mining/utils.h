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

class MeanFeatures {
 public:
  template <typename Iterable>
  static data_mining::Features Calculate(
      const Iterable &set_with_features_interface,
      const int &dimension);
};

}  // namespace data_mining


#include "data_mining/utils-inl.h"


#endif
