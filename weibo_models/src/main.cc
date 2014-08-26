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
using data_mining::ListSharedPtrItemSet;
using data_mining::VecSharedPtrClusterResult;
using data_mining::HierarchyClustering;
using data_mining::MaxSimilarityItemInItemSet;


constexpr const int kDimension = 10;
constexpr const int kMaxProcessSize = 1000;
constexpr const int kPrintSize = 15;


void PrintItemSetInfo(const ListSharedPtrItemSet &item_sets,
                      const double &max_cu_value) {
  cout << "========================================" << endl;
  cout << "Size: " << item_sets.size() << endl;

  if (item_sets.size() > kPrintSize) {
    return;
  }

  for (const auto &item_set : item_sets) {
    cout << "ID: " << item_set->id()
         <<  " Messages: " << item_set->items().size() << endl;
    cout << "Features: ";
    int index = 0;
    for (const auto &feature : item_set->features()) {
      cout << index++ << ":" << feature << ", ";
    }
    cout << endl;
  }
  cout << "Max CU: " << max_cu_value << endl;
}


void PrintClusterResult(const VecSharedPtrClusterResult &results,
                        const vector<string> &raw_messages) {

  cout << "========================================" << endl;
  cout << "Cluster Result" << endl;
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
  for (const auto &result : results) {
    auto item_set = result->GetItemSet();
    auto item = MaxSimilarityItemInItemSet::Find(item_set);
    int id = item->id();
    cout << "ItemSet " << item_set->id() << ": " << endl
         << raw_messages[id] << endl;
    cout << endl;
  }
}


void PrintKeywords(const vector<string> &keywords) {
  int index = 0;
  for (const auto &word : keywords) {
    cout << index++ << ":" << word << " ";
  }
  cout << endl;
}


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

  while (clustering_handler.NotStop()) {
    clustering_handler.SingleMove();
    auto item_sets = clustering_handler.item_sets();
    double max_cu_value = clustering_handler.max_cu_value();

    PrintItemSetInfo(item_sets, max_cu_value);
  }

  auto results = clustering_handler.GetClusterResults();

  PrintClusterResult(results, raw_messages);
  PrintKeywords(keywords);
}
