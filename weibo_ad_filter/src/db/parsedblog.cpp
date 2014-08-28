/**
 * @file    parsedblog.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/28/2014 12:47:48 PM
 * @version 1.0
 *
 */
#include "parsedblog.h"

const string ParsedBlog::WORD_FOR_AT = "WORD_FOR_AT";
SimHash ParsedBlog::sim;
const string ParsedBlog::WORD_FOR_HTTP = "WORD_FOR_HTTP";

ParsedBlog::ParsedBlog(const Blog &b, const std::vector<Word> &Ws) {
  blog = b;
  Words = Ws;
  std::vector<string> ws = Towords();
  _AddSpecialToken(blog.m_content, ws);  /// test symbols like http and @
  fingerprint = sim.BlogHashAfterParser(ws);
}

std::vector<string> ParsedBlog::Towords() {
  std::vector<string> res;
  for(auto w : Words) {
    res.push_back(w.word);
  }
  return res;
}

INSERT_DATA ParsedBlog::ToInsertData() {
  std::ostringstream ss;
  for (auto w : Words){
    ss<< w.word << " " << w.proper << " ";
  }
  INSERT_DATA insertdata;
  insertdata.mid = blog.m_mid;
  insertdata.text = blog.m_content;
  insertdata.spilt = ss.str();
  return insertdata;
}

/**
 * @brief detect whether blog contains "@" or "http://" substring, 
 * if so, add rare charector to words
 */
void ParsedBlog::_AddSpecialToken(const string &m_content, std::vector<string> &words) {
  if (m_content.find("@") != string::npos) {
    words.push_back(WORD_FOR_AT);
  }
  if (m_content.find("http://") != string::npos) {
    words.push_back(WORD_FOR_HTTP);
  }
}

void ParsedBlog::_RemoveSpecialToken(std::vector<string> &words) {
  while (words.back() == WORD_FOR_AT || words.back() == WORD_FOR_HTTP)
    words.pop_back();
}
