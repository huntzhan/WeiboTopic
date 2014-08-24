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
#include "utils/calculator.h"


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


class FeaturesAndMessageIDs : public ClusterResult {
 public:
  FeaturesAndMessageIDs(const Features &features, const IDs &ids);
  Features GetFeatures() const override;
  IDs GetIDs() const override;

 private:
  const Features features_;
  const IDs ids_;
};


class StateKeeper {
 public:
  void Init(const ListSharedPtrItemSet &item_sets);
  void Update(const ListSharedPtrItemSet &item_sets);
  VecSharedPtrClusterResult result_container() const;

 private:
  utils::CatergoryUtilityEvaluator cu_evaluator_;
  double max_cu_values_ = -1.0;
  VecSharedPtrClusterResult result_container_;
};


class HierarchyClustering : public ClusterProcedure {
 public:
  // interfaces.
  void AddItem(const AdapterInterface &adapter) override;
  void Prepare() override;
  void CarryOutCluster() override;
  VecSharedPtrClusterResult GetClusterResults() override;

 private:
  void SingleStepOfClustering();
  ListSharedPtrItemSet item_sets_;
  SimilarityMap similarity_of_item_sets_;
  StateKeeper state_keeper_;
};


}  // namespace data_mining
#endif