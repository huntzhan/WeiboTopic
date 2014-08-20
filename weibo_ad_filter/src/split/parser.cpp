/**
 * @file    parser.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 10:31:56
 * @version 1.0
 *
 */

#include "Textspilt.h"
#include "parser.h"
/**
 *  @brief Constructor
 *
 *  @param
 */
std::set<std::string> Parser::stopwords;
Parser::Parser() {
  if(stopwords.size()<1){
     MakeStopSet(stopwords);
  }
}

/**
 *  @brief Destructor
 *
 *  @param
 */
Parser::~Parser() {
}

/**
 *  @brief MakeStopSet 获得停词
 *
 *  @param
 *  @return
 */
void Parser::MakeStopSet(std::set<std::string> &stopwords) {
  std::ifstream ifile("stopwords.txt");
  while (!ifile.eof()) {
    std::string temp;
    ifile >> temp;
    boost::trim(temp);
    stopwords.insert(temp);
  }
}


/**
 *  @brief LexicalAnalysis
 *
 *  @param
 *  @return
 */
void Parser::LexicalAnalysis(const std::string &rawtext, std::vector<Word> &words) {
  TextSpilt::goodWordArticlePorperty(rawtext,stopwords, words);
}


void Parser::LexicalAnalysisWord(const std::string &rawtext, std::vector<std::string> &words) {
  TextSpilt::goodWordArticle(rawtext,stopwords, words);
}

