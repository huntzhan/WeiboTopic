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

#include "data_mining/interface.h"
#include "utils/dimension_reducer.h"


#ifndef DATA_MINING_ITEM_H_
#define DATA_MINING_ITEM_H_
namespace data_mining {


class AdapterForBitset : public AdapterInterface {
 public:
  template <int T>
  AdapterForBitset(const utils::BitsetFeatures<T> &message, const int &id) {
    id_ = id;
    // 1.0 represents that the item holds the feature, while 0.0 not.
    for (const auto &flag : message) {
      if (flag) {
        features_.push_back(1.0);
      } else {
        features_.push_back(0.0);
      }
    }
  }
  // interface.
  Features GetFeatures() const override;
  int GetID() const override;

 private:
  int id_;
  Features features_;
};


class ItemWithCosineDistance : public ItemInterface {
 public:
  double Similarity(const ItemInterface &other) const override;
};


class ItemSetWithCosineDistance : public ItemSetWithCosineDistance {
 public:
  ItemSetWithCosineDistance(const ItemWithCosineDistance &item);
  double Similarity(const ItemSetInterface &other) const override;
  void Merge(const ItemSetInterface &other) const override;

  std::vector<ItemWithCosineDistance> items() const;

 private:
  void UpdateMeanFeatures() const;
  std::vector<ItemWithCosineDistance> &items_;
};

}  // namespace data_mining
#endif
