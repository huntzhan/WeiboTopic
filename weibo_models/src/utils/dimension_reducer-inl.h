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
#include <vector>
#include <bitset>
#include <sstream>
#include <utility>
#include <iterator>

#include "KeywordExtractor.hpp"

#include "utils/text_cleaner.h"


namespace utils {


template <int dimension>
void TFIDFDimensionReducer<dimension>::Process(const VecStr &messages) {
  utils::TextCleaner cleaner;

  // firstly, clean each messages and keep the result.
  std::vector<std::string> cleaned_messages;
  std::ostringstream all_messages;
  for (const std::string &message : messages) {
    // read all mes
    auto cleaned_message = cleaner.Clean(message);
    cleaned_messages.push_back(cleaned_message);
    all_messages << cleaned_message;
  }

  // secondly, extract keyword based on tf/idf.
  // load dataset. Make sure the path is relative to top-level build directory.
  CppJieba::KeywordExtractor extractor(
      "data/jieba/jieba.dict.utf8",
      "data/jieba/hmm_model.utf8",
      "data/jieba/idf.utf8",
      "data/jieba/stop_words.utf8");
  VecStr keywords;
  extractor.extract(all_messages.str(), keywords, dimension);

  // thirdly, vectorize messages based on keywords and cleaned messages.
  VecBitset<dimension> vectorized_messages;
  for (const std::string &message : cleaned_messages) {
    std::bitset<dimension> vectorized_message;

    // try to find each keyword in message.
    for (auto keyword_iter = keywords.cbegin();
         keyword_iter != keywords.cend(); ++keyword_iter) {
      // get current index.
      auto index = std::distance(keywords.cbegin(), keyword_iter);
      // check existence of keyword.
      if (message.find(*keyword_iter) != std::string::npos) {
        vectorized_message[index] = true;
      } else {
        vectorized_message[index] = false;
      }
    }
    vectorized_messages.push_back(vectorized_message);
  }
  // set data member.
  keywords_ = std::move(keywords);
  vectorized_messages_ = std::move(vectorized_messages);
}


template <int dimension>
VecStr TFIDFDimensionReducer<dimension>::GetKeywords() const {
  return keywords_;
}


template <int dimension>
VecBitset<dimension>
TFIDFDimensionReducer<dimension>::GetVectorizedMessages() const {
  return vectorized_messages_;
}


}  // namespace utils
