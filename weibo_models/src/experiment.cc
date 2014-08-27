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

#include "database/mysql_handler.h"
#include "utils/dimension_reducer.h"


using std::cout;
using std::endl;
using std::distance;
using std::vector;

using mysql_handler::TopicHandler;
using utils::TFIDFDimensionReducer;
using utils::VecBitset;


constexpr const int kDimension = 30;


vector<double> evaluate_sparse(const VecBitset &vectorized_messages) {
  vector<double> sparse = {-1};
  int all_ones_size = 0;

  for (int dimension = 1; dimension != kDimension + 1; ++dimension) {
    int target_index = dimension - 1;
    for (const auto &message : vectorized_messages) {
      all_ones_size += message[target_index] ? 1 : 0;
    }
    double avg_sparse = static_cast<double>(all_ones_size)
        / (dimension * vectorized_messages.size);
    sparse.push_back(avg_sparse);
  }
  return sparse;
}


int main() {
  TopicHandler handler("split", "Topic_20140823_82");
  handler.Init();
  auto raw_messages = handler.GetMessages();
  cout << "Got messages." << endl;

  TFIDFDimensionReducer<kDimension> reducer;
  reducer.Process(raw_messages);
  auto keywords = reducer.GetKeywords();
  auto vectorized_messages = reducer.GetVectorizedMessages();
  cout << "Vectorized messages." << endl;

  auto sparse = evaluate_sparse(vectorized_messages);
  for (auto iter = sparse.cbegin() + 1; iter != sparse.cend(); ++iter) {
    cout << distance(sparse.cbegin(), iter) << ": " << *iter << endl;
  }
}
