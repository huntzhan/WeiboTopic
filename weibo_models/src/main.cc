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

// debug.
#include <iostream>
using std::cout;
using std::endl;

using std::distance;

using mysql_handler::TopicHandler;
using utils::TFIDFDimensionReducer;
using data_mining::AdapterForBitset;
using data_mining::HierarchyClustering;


constexpr const int kDimension = 20;
constexpr const int kMaxProcessSize = 1000;


int main() {
  TopicHandler handler("testcase", "SingleTopic");
  handler.Init();
  auto raw_messages = handler.GetMessages();
  cout << "Got messages." << endl;

  TFIDFDimensionReducer<kDimension> reducer;
  reducer.Process(raw_messages);
  auto keywords = reducer.GetKeywords();
  auto vectorized_messages = reducer.GetVectorizedMessages();
  cout << "Vectorized messages." << endl;

  decltype(vectorized_messages) nonzero_messages;
  for (const auto &message : vectorized_messages) {
    if (message.none()) {
      continue;
    }
    if (nonzero_messages.size() >= kMaxProcessSize) {
      break;
    }
    nonzero_messages.push_back(message);
  }

  HierarchyClustering clustering_handler;
  for (auto message_iter = nonzero_messages.cbegin();
       message_iter != nonzero_messages.cend(); ++message_iter) {
    // remove all-zero message.
    int id = distance(vectorized_messages.cbegin(), message_iter);
    AdapterForBitset adapter(*message_iter, id);
    clustering_handler.AddItem(adapter);
  }
  cout << "Loaded up" << endl;

  clustering_handler.Prepare();
  cout << "Prepared up" << endl;
  clustering_handler.CarryOutCluster();

  cout << "========================================" << endl;
  cout << "Cluster Result" << endl;
  auto results = clustering_handler.GetClusterResults();
  // debug.
  for (const auto &result : results) {
    auto item_set = result->GetItemSet();
    cout << "ID: " << item_set->id()
         <<  " Messages: " << item_set->items().size() << endl;
    cout << "Features: ";
    int index = 0;
    for (const auto &feature : item_set->features()) {
      cout << index++ << ":" << feature << ", ";
    }
    cout << endl;
  }
  cout << "========================================" << endl;
  // end debug.

  int index = 0;
  for (const auto &word : keywords) {
    cout << index++ << ":" << word << " ";
  }
  cout << endl;

}
