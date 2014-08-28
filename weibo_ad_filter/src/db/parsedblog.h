/**
 * @file    parsedblog.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/28/2014 12:46:58 PM
 * @version 1.0
 *
 */

#ifndef parsedblog_INC
#define parsedblog_INC

#include <list>
#include <sstream>
#include <vector>
#include "model.h"
#include "simhash/simhash.h"
/**
 *  @brief ParsedBlog 
 */
class ParsedBlog {
  public:
    std::vector<string> Towords();
    unsigned GetFingerPrint();
    INSERT_DATA ToInsertData();

    // ====================  LIFECYCLE     ==================================
    ParsedBlog(Blog &b, std::vector<Word> Ws);
    ~ParsedBlog() {}
    ParsedBlog(const ParsedBlog& pb) {
      blog = pb.blog;
      Words = pb.Words;
      fingerprint = pb.fingerprint;
    }

  private:
    void _AddSpecialToken(const string &m_content, std::vector<string> &words);
    void _RemoveSpecialToken(std::vector<string> &words);

    SimHash sim;
    Blog blog;
    std::vector<Word> Words;
    unsigned fingerprint;
    static const string WORD_FOR_AT;
    static const string WORD_FOR_HTTP;

    // DISALLOW_COPY_AND_ASSIGN
    void operator=(const ParsedBlog&);
};
#endif
