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


#ifndef DATA_MINING_INTERFACE_H_
#define DATA_MINING_INTERFACE_H_
namespace data_mining {


// type to contain features.
using Features = std::vector<double>;
using IDs = std::vector<int>;


class AdapterInterface {
 public:
  virtual Features GetFeatures() const = 0;
  virtual int GetID() const = 0;
};


class Property {
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
}


class ItemInterface : public Property {
 public:
  // interface to calculate similarity between two items.
  virtual double Similarity(const ItemInterface &other) const = 0;
};


class ItemSetInterface : public Property {
 public:
  virtual double Similarity(const ItemSetInterface &other) const = 0;
  virtual void Merge(const ItemSetInterface &other) = 0;
};


class ClusterResult {
 public:
  virtual Features GetFeatures() const = 0;
  virtual IDs GetIDs() const = 0;
};


class ClusterProcedure {
 public:
  // interface to define cluster algorithm.
  virtual void AddItem(const AdapterInterface &adapter) = 0;
  virtual void Prepare() = 0;
  virtual void CarryOutCluster() = 0;
  virtual std::vector<ClusterResult> GetClusterResults() = 0;
};


}  // namespace data_mining
#endif
