// ============================================================================
//
//       Filename:  interface.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  09/02/2014 10:02:23
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include <vector>
#include <set>
#include <cstddef>
#include <string>
#include <sstream>
#include <iterator>
#include <tuple>
#include <fstream>
#include <random>

#include "database/mysql_handler.h"
#include "utils/dimension_reducer.h"
#include "data_mining/item_related.h"
#include "data_mining/cluster_related.h"
#include "data_mining/utils.h"

// debug.
#include <iostream>
using std::cout;
using std::endl;

using std::size_t;
using std::ofstream;
using std::distance;
using std::vector;
using std::set;
using std::string;
using std::to_string;
using std::getline;
using std::istringstream;
using std::ostringstream;
using std::tuple;
using std::make_tuple;
using std::shuffle;
using std::default_random_engine;

using mysql_handler::TopicHandler;
using mysql_handler::SubTopicHandler;
using mysql_handler::VecMessagePair;

using utils::VecBitset;
using utils::TFIDFDimensionReducer;

using data_mining::SharedPtrItem;
using data_mining::SharedPtrItemSet;
using data_mining::VecSharedPtrItem;
using data_mining::AdapterForBitset;
using data_mining::ListSharedPtrItemSet;
using data_mining::VecSharedPtrClusterResult;
using data_mining::HierarchyClustering;
using data_mining::MaxEvaluationItemInItemSet;


constexpr const int kDimension = 10;
constexpr const int kMaxProcessSize = 500;
constexpr const int kPrintSize = 50;


template <typename Iterable>
void reorder(Iterable *container_ptr) {
  default_random_engine dre;
  shuffle(container_ptr->begin(), container_ptr->end(),
          dre);
}


tuple<string, string> ExtractDBAndTable(const string &text) {
  istringstream in(text);
  string database_name, table_name;
  getline(in, database_name, '.');
  getline(in, table_name);
  return make_tuple(database_name, table_name);
}


ListSharedPtrItemSet ExtractItemSetsFromResult(
    const VecSharedPtrClusterResult &results) {
  ListSharedPtrItemSet item_sets;
  for (const auto &result : results) {
    auto item_set = result->GetItemSet();
    item_sets.push_back(item_set);
  }
  return item_sets;
}


VecSharedPtrItem GetRepresentingItems(
    const ListSharedPtrItemSet &item_sets,
    const SharedPtrItemSet &item_set) {
  return MaxEvaluationItemInItemSet::TopK(item_sets, item_set, 10);
}


tuple<string, string> PackupMessagePair(
    const SharedPtrItem &item,
    const vector<string> &raw_messages,
    const vector<string> &keywords) {
  int id = item->id();
  // extract text.
  string text = raw_messages[id];
  // extract keywords.
  auto features = item->features();
  ostringstream keyword_text;
  for (size_t index = 0; index != features.size(); ++index) {
    bool has_feature = features[index];
    if (has_feature) {
      keyword_text << keywords[index];
    } else {
      continue;
    }
    if (index != features.size() - 1) {keyword_text << " "; }
  }
  return make_tuple(text, keyword_text.str());
}


string WriteToDB(
    const string &input_database_name,
    const string &input_table_name,
    const int &index,
    const VecMessagePair &message_pairs) {
  // setup database.
  const string &new_table_name =
      "Sub_" + to_string(index) + "_" + input_table_name;
  SubTopicHandler database_handler(input_database_name, new_table_name);
  database_handler.Init();
  // write to db.
  database_handler.StoreSubTopic(message_pairs);
  // return table naem.
  return new_table_name;
}


void StoreTables(const string &input_database_name,
                 const string &input_table_name,
                 const string &output_file_path,
                 const VecSharedPtrClusterResult &results,
                 const vector<string> &raw_messages,
                 const vector<string> &keywords) {
  auto item_sets = ExtractItemSetsFromResult(results);

  vector<string> output_table_names;
  int index = 0;
  for (const auto &item_set: item_sets) {
    auto items = GetRepresentingItems(item_sets, item_set);
    // formated output.
    VecMessagePair output_message_pairs;
    for (const auto &item : items) {
      output_message_pairs.push_back(
          PackupMessagePair(item, raw_messages, keywords));
    }
    // write to db.
    const string &new_table_name = WriteToDB(
        input_database_name, input_table_name,
        index, output_message_pairs);
    ++index;
    output_table_names.push_back(new_table_name);
  }
  // output new table names..
  ofstream out(output_file_path);
  for (size_t index = 0; index != output_table_names.size(); ++index) {
    // casue I've used the same database.
    out << input_database_name << "." << output_table_names[index];
    if (index != output_table_names.size() - 1) {
      out << ",";
    }
    out << endl;
  }
  out.close();
}


int main(int args, char **argv) {
  if (args != 3) {
    cout << "Error argument size." << endl;
    return -1;
  }
  // extract arguments.
  const string output_file_path = argv[1];
  auto name_pair = ExtractDBAndTable(argv[2]);
  const string database_name = std::get<0>(name_pair);
  const string table_name = std::get<1>(name_pair);

  // load data.
  TopicHandler handler(database_name, table_name);
  handler.Init();
  auto raw_messages = handler.GetMessages();
  reorder(&raw_messages);
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
    // auto item_sets = clustering_handler.item_sets();
    // double max_cu_value = clustering_handler.max_cu_value();
    // PrintItemSetInfo(item_sets, max_cu_value);
  }

  auto results = clustering_handler.GetClusterResults();

  // PrintClusterResult(results, raw_messages, vectorized_messages, keywords);
  StoreTables(database_name, table_name,
              output_file_path,
              results, raw_messages, keywords);
}
