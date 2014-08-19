/**
 * @file    parser.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 */

#ifndef parser_INC
#define parser_INC

#include "split/Textspilt.h"

/**
 *  @brief Parser 
 */
class Parser {
public:
  // ====================  LIFECYCLE     ==================================
  Parser();
  ~Parser();
  void LexicalAnalysis(const std::string &rawtext, std::vector<Word> &words);

private:
  void MakeStopSet();

  TextSpilt split;
  std::set<std::string> stopwords;

  // DISALLOW_COPY_AND_ASSIGN
  Parser(const Parser&);
  void operator=(const Parser&);
};
#endif

