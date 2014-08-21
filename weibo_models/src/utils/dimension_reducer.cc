// ============================================================================
//
//       Filename:  dimension_reducer.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/21/2014 15:13:43
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================
#include <string>
#include <sstream>
#include <utility>

#include "KeywordExtractor.hpp"


namespace utils {


template <int T>
void TFIDFDimensionReducer<T>::Process(const VecStr &messages) {
  std::ostringstream all_messages;
  for (const std::string &message : messages) {
    // read all mes
    all_messages << message;
  }

  // load dataset. Make sure the path is relative to top-level build directory.
  CppJieba::KeywordExtractor extractor(
      "data/jieba/jieba.dict.utf8",
      "data/jieba/hmm_model.utf8",
      "data/jieba/idf.utf8",
      "data/jieba/stop_words.utf8");
  VecStr keywords;
  extractor.extract(all_messages.str(), keywords, T);
  // move assignment.
  keywords_ = std::move(keywords);
}


template <int T>
VecStr TFIDFDimensionReducer<T>::GetKeywords() {
  return keywords_;
}


template <int T>
VecBitset<T> TFIDFDimensionReducer<T>::GetVectorizedMessages() {

}

}  // namespace utils
