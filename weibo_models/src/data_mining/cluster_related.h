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
#define DATA_MINING_CLUSTER_RELITEM_H_
namespace data_mining {


using ItemSetRefPair = std::pair<
  std::reference_wrapper<const ItemSetInterface>,
  std::reference_wrapper<const ItemSetInterface>>;

// compare function for ItemSetInterface.
auto item_set_compare = [](const ItemSetInterface &a,
                           const ItemSetInterface &b) {
  return a.id() < b.id();
}


class AuxiliaryFunc {
 public:
  static ItemSetRefPair MakeItemSetRefPair(
      const ItemInterface &item_a, const ItemInterface &item_a);

  static ListRefItemSets::iterator BinarySearchListRefItemSets(
      ListRefItemSets *item_sets, const ItemSetInterface &item_set);

  static void InsertItemSetToListRefItemSets(
      ListRefItemSets *item_sets, const ItemSetInterface &item_set);
};


class HierarchyClustering : public ClusterProcedure {
 public:
  // interfaces.
  void AddItem(const AdapterInterface &adapter) override;
  void Prepare() override;
  void CarryOutCluster() override;
  std::vector<ClusterResult> GetClusterResults() override;

 private:
  ListRefItemSets item_sets_;
  std::map<ItemSetRefPair, double> similarity_of_item_sets_;
};


}  // namespace data_mining
#endif
