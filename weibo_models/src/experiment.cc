// ============================================================================
//
//       Filename:  experiment.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/27/2014 12:30:01
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include <vector>
#include <iterator>
#include <iostream>
#include <string>

#include "database/mysql_handler.h"
#include "utils/dimension_reducer.h"


using std::cout;
using std::endl;
using std::distance;
using std::vector;
using std::string;
using std::to_string;

using mysql_handler::TopicHandler;
using utils::TFIDFDimensionReducer;
using utils::VecBitset;


constexpr const int kDimension = 30;


vector<double> evaluate_sparse(
    const VecBitset<kDimension> &vectorized_messages) {
  vector<double> sparse = {-1};
  int all_ones_size = 0;

  for (int dimension = 1; dimension != kDimension + 1; ++dimension) {
    int target_index = dimension - 1;
    for (const auto &message : vectorized_messages) {
      all_ones_size += message[target_index] ? 1 : 0;
    }
    double avg_sparse = static_cast<double>(all_ones_size)
        / (dimension * vectorized_messages.size());
    sparse.push_back(avg_sparse);
  }
  return sparse;
}


int main() {
  for (int table_id = 1; table_id != 100; ++table_id) {
    string table_name = "Topic_20140827_" + to_string(table_id);
    TopicHandler handler("split", table_name);
    handler.Init();
    auto raw_messages = handler.GetMessages();

    TFIDFDimensionReducer<kDimension> reducer;
    reducer.Process(raw_messages);
    auto keywords = reducer.GetKeywords();
    auto vectorized_messages = reducer.GetVectorizedMessages();

    auto sparse = evaluate_sparse(vectorized_messages);
    // cout << "Table:" << table_name << endl;
    for (auto iter = sparse.cbegin() + 1; iter != sparse.cend(); ++iter) {
      // cout << distance(sparse.cbegin(), iter) << ":" << *iter << endl;
      cout << *iter << '\t';
    }
    cout << endl;
  }
}
