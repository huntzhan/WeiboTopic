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

#include <vector>
#include <set>
#include <string>
#include <iterator>

#include "database/mysql_handler.h"
#include "utils/dimension_reducer.h"
#include "data_mining/item_related.h"
#include "data_mining/cluster_related.h"
#include "data_mining/utils.h"

// debug.
#include <iostream>
using std::cout;
using std::endl;

using std::distance;
using std::vector;
using std::set;
using std::string;

using mysql_handler::TopicHandler;
using utils::TFIDFDimensionReducer;
using data_mining::AdapterForBitset;
using data_mining::HierarchyClustering;
using data_mining::MaxSimilarityItemInItemSet;


constexpr const int kDimension = 10;
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

  vector<int> ids;
  set<string> unique_string;

  for (auto message_iter = vectorized_messages.cbegin();
       message_iter != vectorized_messages.cend(); ++message_iter) {
    if (message_iter->none()) {
      continue;
    }
    if (ids.size() >= kMaxProcessSize) {
      break;
    }
    int id = distance(vectorized_messages.cbegin(), message_iter);

    if (unique_string.count(raw_messages[id]) != 0) {
      continue;
    } else {
      // update unique_string.
      unique_string.insert(raw_messages[id]);
    }
    // only save non-empty, unique messages.
    ids.push_back(id);
  }

  HierarchyClustering clustering_handler;
  for (const int &id : ids) {
    // remove all-zero message.
    AdapterForBitset adapter(vectorized_messages[id], id);
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
    cout << endl << "Contains: " << endl;
    index = 0;
    for (const auto &item : item_set->items()) {
      if (index == 10) break;
      int id = item->id();
      cout << index << ": " << raw_messages[id] << endl;
      ++index;
    }
  }
  cout << "========================================" << endl;
  // end debug.

  int index = 0;
  for (const auto &word : keywords) {
    cout << index++ << ":" << word << " ";
  }
  cout << endl;

  cout << "========================================" << endl;
  for (const auto &result : results) {
    auto item_set = result->GetItemSet();
    auto item = MaxSimilarityItemInItemSet::Find(item_set);
    int id = item->id();
    cout << raw_messages[id] << endl;
    cout << endl;
  }
}
