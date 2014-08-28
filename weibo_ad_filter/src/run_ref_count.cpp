/**
 * @file    run_ref_count.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/28/2014 09:28:10 AM
 * @version 1.0
 *
 */


#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "allocator/allocator.h"
#include "logger/log.h"
#include "split/parser.h"
#include "db/model.h"
#include "db/parsedblog.h"
#include "ref_count/ref.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

const unsigned ROWS_EACH_TIME = 8000;

bool IsMeaninglessBlog(ParsedBlog &pb);

int main() {
  TextSpilt::init_ICTCAL();
  Parser parser;
  RefCount ref;
  std::list<ParsedBlog> parsed_blogs;

  Allocator allo("Microblog1408215600");
  if (allo.HasNextTable()) {
  // while (allo.HasNextTable()) {
    allo.NextTable();
    Log::Logging(RUN_T, "###Start Table " + allo.GetCurrentTableName() + ": " + std::to_string(allo.GetRowsOfCurrentTable()));
    while (allo.HasNextRow()) {
      std::list<Blog> blogs;
      unsigned count = allo.NextBlogs(ROWS_EACH_TIME, blogs);
      Log::Logging(RUN_T, "get rows from crawler: " + to_string(count));
      for(auto &blog : blogs) {
        /// Lexcal Analysis by ICTCLAS50
        std::vector<Word> words;
        parser.LexicalAnalysis(blog.m_content.c_str(), words);
        ParsedBlog pb(blog, words);
        if (IsMeaninglessBlog(pb))
          continue;
        unsigned fp = pb.fingerprint_();
        ref.AddFingerPrint(fp, 1);
        parsed_blogs.push_back(pb);
      }
    }
    Log::Logging(RUN_T, "###Table " + allo.GetCurrentTableName() + ": " + std::to_string(allo.GetRowsOfCurrentTable()));
    Log::Logging(RUN_T, "###Ref Objects: " + std::to_string(ref.GetRefSize()));
    for (auto i : parsed_blogs) {
      unsigned int pf;
      unsigned int count = ref.GetRefCount(i.fingerprint_(), 1, pf);
      Log::Logging(REF_DIST_1_T, Blog2Str(i.blog_()) + ">" + to_string(pf) + ">" + to_string(count));
    }
    parsed_blogs.clear();
  }
}

bool IsMeaninglessBlog(ParsedBlog &pb) {
  const std::vector<Word> &Ws = pb.Words_();
  const Blog &blog = pb.blog_();
  if (Ws.size() == 1 && Ws.front().word.empty()) {
    return true;
  }
  else if (Ws.size() <= 6) {
    Log::Logging(TOO_SHORT_T, std::to_string(Ws.size()) + " " + blog.m_content);
    return true;
  }
  else return false;
}
// void DoRefCount() {
//   /// ref count tactic
//   RefCount ref;
//   for (auto data : datas) {
//     /// ref count detection
//     vector<string> ws = WordsToStrs(data.words);
//     AddSpecialToken(data.blog.m_content, ws);  /// test symbols like http and @
//     ref.AddFingerPrint(ws, 1);
//     RemoveSpecialToken(ws);
//   }
//   for (auto data : datas) {
//     vector<string> ws = WordsToStrs(data.words);
//     AddSpecialToken(data.blog.m_content, ws);  /// test symbols like http and @
//     unsigned int pf;
//     unsigned int count = ref.GetRefCount(ws, 1, pf);
//     Log::Logging(REF_DIST_1_T, Blog2Str(data.blog) + ">" + to_string(pf) + ">" + to_string(count));
//   }
// }
