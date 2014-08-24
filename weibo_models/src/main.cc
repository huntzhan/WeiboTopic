// ============================================================================
//
//       Filename:  main.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/17/2014 23:58:14
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (zhx), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include <iterator>

#include "database/mysql_handler.h"
#include "utils/dimension_reducer.h"
#include "data_mining/item_related.h"
#include "data_mining/cluster_related.h"


using std::distance;

using mysql_handler::TopicHandler;
using utils::TFIDFDimensionReducer;
using data_mining::AdapterForBitset;
using data_mining::HierarchyClustering;


int main() {
  TopicHandler handler("testcase", "SingleTopic");
  handler.Init();
  auto raw_messages = handler.GetMessages();

  TFIDFDimensionReducer<20> reducer;
  reducer.Process(raw_messages);
  auto keywords = reducer.GetKeywords();
  auto vectorized_messages = reducer.GetVectorizedMessages();

  HierarchyClustering clustering_handler;
  for (auto message_iter = vectorized_messages.cbegin();
       message_iter != vectorized_messages.cend(); ++message_iter) {
    int id = distance(vectorized_messages.cbegin(), message_iter);
    AdapterForBitset adapter(*message_iter, id);
    cluster_related.AddItem(adapter);
  }

  clustering_handler.Prepare();
  clustering_handler.CarryOutCluster();
}
