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
using std::string;
using std::cout;
using std::endl;
using std::vector;

const unsigned ROWS_EACH_TIME = 5000;

int main() {
  Allocator allo("Microblog1408215600");
  while (allo.HasNextTable()) {
    allo.NextTable();
    std::list<Blog> blogs;
    while (allo.HasNextRow()) {
      unsigned count = allo.NextBlogs(5000, blogs);
      Log::Logging(RUN_T, "get rows from crawler: " + to_string(count));
        for(auto &blog : blogs) {
          bool is_good_blog = pre.PerformTacticOnBlog(blog);
          /// Lexcal Analysis by ICTCLAS50
          std::vector<Word> words;
          parser.LexicalAnalysis(blog.m_content.c_str(), words);
          bool is_good_parsed_blog = pre.PerformTacticOnParsedBlog(words);
          if (!is_good_blog) {
            Log::LoggingRandom(ZOMBIE_T, 1000, Blog2Str(blog));
          }
          if (!is_good_parsed_blog) {
            Log::LoggingRandom(ZOMBIE_SIM_T, 100, Blog2Str(blog));
          }
          if(is_good_blog && is_good_parsed_blog){
            /// change parsed blog into insert_data
            InsertData data(blog, words);
            datas.push_back(data);
            // INSERT_DATA data = PackInsertData(blog, words);
            // if(data.text.length()<2 || data.spilt.length()<2 || data.text.empty() || data.spilt.empty()){
            //   //std::cout << insertdata.text<<std::endl;
            //   ;
            // }
            // else
            //   prepare_datas.push_back(data);
          }
        }
    }
  }
}
