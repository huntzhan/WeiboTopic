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
#include "data_mining/utils.h"


#ifndef DATA_MINING_CLUSTER_RELATED_H_
#define DATA_MINING_CLUSTER_RELATED_H_
namespace data_mining {


using SharedPtrItemSetPair = std::pair<SharedPtrItemSet, SharedPtrItemSet>;
using SimilarityMap = std::map<SharedPtrItemSetPair, double>;


class AuxiliaryFunc {
 public:
  static SharedPtrItemSetPair MakeItemSetPair(
      const SharedPtrItemSet &set_a,
      const SharedPtrItemSet &set_b);

  static ListSharedPtrItemSet::iterator SearchItemSet(
      ListSharedPtrItemSet *item_sets,
      const SharedPtrItemSet &item_set);

  static void RemoveItemSet(
      ListSharedPtrItemSet *item_sets,
      const ListSharedPtrItemSet::iterator &iter);

  static void InsertItemSet(
      ListSharedPtrItemSet *item_sets,
      const SharedPtrItemSet &item_set);

  static SharedPtrItemSetPair FindMaxSimilarity(
      SimilarityMap *similarity_map);
};


class ClusterCached : public ClusterResult {
 public:
  ClusterCached(const SharedPtrItemSet &item_set);
  SharedPtrItemSet GetItemSet() const;

 private:
  SharedPtrItemSet cached_item_set_ = nullptr;
};


class StateKeeper {
 public:
  void Init(const ListSharedPtrItemSet &item_sets);
  void Update(const ListSharedPtrItemSet &item_sets);
  VecSharedPtrClusterResult GetClusterResults() const;

  double max_cu_value() const { return max_cu_value_; }

 private:
  CatergoryUtilityEvaluator cu_evaluator_;
  double max_cu_value_ = -1.0;
  VecSharedPtrClusterResult cached_item_sets_;
};


class HierarchyClustering : public ClusterProcedure {
 public:
  // interfaces.
  void AddItem(const AdapterInterface &adapter) override;
  void Prepare() override;
  bool NotStop() const override;
  void SingleMove() override;
  VecSharedPtrClusterResult GetClusterResults() override;

  ListSharedPtrItemSet item_sets() const { return item_sets_; }
  double max_cu_value() const { return state_keeper_.max_cu_value(); }

 private:
  ListSharedPtrItemSet item_sets_;
  SimilarityMap similarity_of_item_sets_;
  StateKeeper state_keeper_;
};


}  // namespace data_mining
#endif
