// ============================================================================
//
//       Filename:  item.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/22/2014 17:17:09
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <vector>
#include <cstddef>

#include "data_mining/interface.h"
#include "utils/dimension_reducer.h"


#ifndef DATA_MINING_ITEM_RELATED_H_
#define DATA_MINING_ITEM_RELATED_H_
namespace data_mining {


class AdapterForBitset : public AdapterInterface {
 public:
  template <std::size_t dimension>
  AdapterForBitset(
      const utils::BitsetFeatures<dimension> &message, const int &id);
  // interface.
  Features GetFeatures() const override;
  int GetID() const override;

 private:
  int id_;
  Features features_;
};


class ItemWithCosineDistance : public ItemInterface {
 public:
  // interface.
  double Similarity(const SharedPtrItem &other) const override;
};


class ItemSetWithCosineDistance : public ItemSetInterface {
 public:
  ItemSetWithCosineDistance(const SharedPtrItem &item);
  // interface.
  double Similarity(const SharedPtrItemSet &other) const override;
  void Merge(const SharedPtrItemSet &other) override;
  // accessor.

 private:
  void UpdateMeanFeatures();
};


}  // namespace data_mining


// inline template.
namespace data_mining {


template <std::size_t dimension>
AdapterForBitset::AdapterForBitset(
    const utils::BitsetFeatures<dimension> &message, const int &id) {
  id_ = id;
  // 1.0 represents that the item holds the feature, while 0.0 not.
  for (std::size_t index = 0; index != message.size(); ++index) {
    if (message[index]) {
      features_.push_back(1.0);
    } else {
      features_.push_back(0.0);
    }
  }
}


}  // namespace data_mining
#endif
