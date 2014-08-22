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
#include <bitset>
#include <sstream>
#include <utility>

#include "KeywordExtractor.hpp"

#include "utils/text_cleaner.h"


namespace utils {


template <int T>
void TFIDFDimensionReducer<T>::Process(const VecStr &messages) {
  utils::TextCleaner cleaner;
  
  std::ostringstream all_messages;
  for (const std::string &message : messages) {
    // read all mes
    all_messages << cleaner.Clean(message);
  }

  // load dataset. Make sure the path is relative to top-level build directory.
  CppJieba::KeywordExtractor extractor(
      "data/jieba/jieba.dict.utf8",
      "data/jieba/hmm_model.utf8",
      "data/jieba/idf.utf8",
      "data/jieba/stop_words.utf8");
  VecStr keywords;
  extractor.extract(all_messages.str(), keywords, T);

  VecBitset<T> vectorized_messages;
  for (const std::string &message : messages) {
    int index = 0;
    std::bitset<T> vectorized_message;
    // try to find each keyword in message.
    for (const std::string &keyword : keywords) {
      if (message.find(keyword) != std::string::npos) {
        vectorized_message[index] = true;
      } else {
        vectorized_message[index] = false;
      }
      ++index;
    }
    vectorized_messages.push_back(vectorized_message);
  }
  // set data member.
  keywords_ = std::move(keywords);
  vectorized_messages_ = std::move(vectorized_messages);
}


template <int T>
VecStr TFIDFDimensionReducer<T>::GetKeywords() {
  return keywords_;
}


template <int T>
VecBitset<T> TFIDFDimensionReducer<T>::GetVectorizedMessages() {
  return vectorized_messages_;
}

}  // namespace utils
