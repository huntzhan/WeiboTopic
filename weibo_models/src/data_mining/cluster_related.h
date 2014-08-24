// ============================================================================
//
//       Filename:  cluster_related.h
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/23/2014 17:33:42
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <vector>
#include <utility>
#include <functional>

#include "data_mining/interface.h"
#include "data_mining/item_related.h"


#ifndef DATA_MINING_CLUSTER_RELATED_H_
#define DATA_MINING_CLUSTER_RELATED_H_
namespace data_mining {

using SharedPtrItemSetPair = std::pair<SharedPtrItemSet, SharedPtrItemSet>;

// compare function for ItemSetInterface.
auto item_set_compare = [](const SharedPtrItemSet &a,
                           const SharedPtrItemSet &b) {
  return a->id() < b->id();
};


class AuxiliaryFunc {
 public:
  static SharedPtrItemSetPair MakeItemSetPair(
      const SharedPtrItemSet &set_a, const SharedPtrItemSet &set_b);

  static ListSharedPtrItemSet::iterator SearchItemSet(
      ListSharedPtrItemSet *item_sets,
      const SharedPtrItemSet &item_set);

  static void RemoveItemSet(
      ListSharedPtrItemSet *item_sets,
      const ListSharedPtrItemSet::iterator &iter);

  static void InsertItemSet(
      ListSharedPtrItemSet *item_sets,
      const SharedPtrItemSet &item_set);
};


class HierarchyClustering : public ClusterProcedure {
 public:
  // interfaces.
  void AddItem(const AdapterInterface &adapter) override;
  void Prepare() override;
  void CarryOutCluster() override;
  std::vector<ClusterResult> GetClusterResults() override;

 private:
  ListSharedPtrItemSet item_sets_;
  std::map<SharedPtrItemSetPair, double> similarity_of_item_sets_;
};


}  // namespace data_mining
#endif
