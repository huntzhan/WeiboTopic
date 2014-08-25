// ============================================================================
//
//       Filename:  interface.h
//
//    Description:  Defines interfaces of n-feature items, meaning that the
//                  targets of clustering should share the same features.
//
//        Version:  1.0
//        Created:  08/22/2014 15:54:18
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include <vector>
#include <list>
#include <memory>

#ifndef DATA_MINING_INTERFACE_H_
#define DATA_MINING_INTERFACE_H_
namespace data_mining {


// type to contain features.
using Features = std::vector<double>;
using IDs = std::vector<int>;

class ItemInterface;
class ItemSetInterface;
class ClusterResult;

using SharedPtrItem = std::shared_ptr<ItemInterface>;
using SharedPtrItemSet = std::shared_ptr<ItemSetInterface>;
using SharedPtrClusterResult = std::shared_ptr<ClusterResult>;

using VecSharedPtrItem = std::vector<SharedPtrItem>;
using ListSharedPtrItemSet = std::list<SharedPtrItemSet>;
using VecSharedPtrClusterResult = std::vector<SharedPtrClusterResult>;


class AdapterInterface {
 public:
  virtual Features GetFeatures() const = 0;
  virtual int GetID() const = 0;
};


class ItemProperty {
 public:
  // accessor.
  int id() const { return id_; }
  Features features() const { return features_; }
  // mutator.
  void set_id(const int &id) { id_ = id; }
  void set_features(const Features &features) { features_ = features; }

 private:
  Features features_;
  int id_;
};


class ItemInterface : public ItemProperty {
 public:
  // interface to calculate similarity between two items.
  virtual double Similarity(const SharedPtrItem &other) const = 0;
};


class ItemSetProperty : public ItemProperty {
 public:
  // accessor.
  VecSharedPtrItem items() const { return items_; }
  // mutator.
  void set_items(const VecSharedPtrItem &items) { items_ = items; }
  void add_item(const SharedPtrItem &item) { items_.push_back(item); }

 private:
  VecSharedPtrItem items_;
};


class ItemSetInterface : public ItemSetProperty {
 public:
  virtual double Similarity(const SharedPtrItemSet &other) const = 0;
  virtual void Merge(const SharedPtrItemSet &other) = 0;
};


class ClusterResult {
 public:
  virtual SharedPtrItemSet GetItemSet() const = 0;
};


class ClusterProcedure {
 public:
  // interface to define cluster algorithm.
  virtual void AddItem(const AdapterInterface &adapter) = 0;
  virtual void Prepare() = 0;
  virtual void CarryOutCluster() = 0;
  virtual VecSharedPtrClusterResult GetClusterResults() = 0;
};


}  // namespace data_mining
#endif
