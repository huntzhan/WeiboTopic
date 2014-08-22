// ============================================================================
//
//       Filename:  text_cleaner.cc
//
//    Description:  
//
//        Version:  1.0
//        Created:  08/21/2014 17:46:10
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Zhan Haoxun (huntzhan), programmer.zhx@gmail.com
//   Organization:  
//
// ============================================================================

#include "utils/text_cleaner.h"

#include <string>
#include <vector>

#include <boost/regex.hpp>

using std::string;
using std::vector;
using boost::regex;
using boost::regex_replace;

namespace utils {

string TextCleaner::Clean(string text) {
  vector<string> rules = {
    "http://.+?/\\w+",
    "#|:"};

  for (const string &rule : rules) {
    regex pattern(rule);
    text = regex_replace(text, pattern, "");
  }
  return text;
}

}  // namespace utils
