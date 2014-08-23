/**
 * @file    preprocessor.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 10:16:45
 * @version 1.0
 *
 * @section LICENSE
 *
 */

#ifndef preprocessor_INC
#define preprocessor_INC

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <set>
#include "db/model.h"
#include "tactic.h"
#include "simhash/simhash.h"
#include "database/mysql_handler.h"
#include "split/Textspilt.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::set;
/**
 *  @brief Preprocessor return true if the blog is useful
 */
class Preprocessor {
  public:
    // ====================  LIFECYCLE     ==================================
    Preprocessor() { 
      handler = std::make_shared<mysql_handler::SpamHandler>("simhash", "spam");
      handler->Init(); 
    }
    ~Preprocessor() {
      if(! fingerprint.empty())
        FlushCachedFingerprint(HAMMING_DISTANCE);
    }
    bool PerformTacticOnBlog(const Blog& b);
    bool PerformTacticOnParsedBlog(const vector<Word> &words);

  private:
    ZombieTactic t_zombie;
    /// TopicTcatic t_topic;
    UserTactic t_user;

    bool IsParsedBlogInFingerprints(const vector<string> &words, const int dist);
    bool IsSimhashValuesInDB(vector<unsigned int> v);
    void FlushCachedFingerprint(const int dist);
    void AddFingerPrint(const Blog &b, const int dist);
    void Flush(vector<unsigned int> &v);
    /// mysql_handler::SpamHandler handler("simhash", "spam");
    std::shared_ptr<mysql_handler::SpamHandler> handler;
    set<unsigned int> fingerprint;
    SimHash sim;
    const size_t FLUSH_DB_THRED = 10000;
    const int HAMMING_DISTANCE = 1;

    // DISALLOW_COPY_AND_ASSIGN
    Preprocessor(const Preprocessor&);
    void operator=(const Preprocessor&);
};
#endif
