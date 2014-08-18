#ifndef _TEXTSPILT_H
#define _TEXTSPILT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include "ICTCLAS50.h"
#include <locale>
typedef struct _Word {
  std::string word;
  std::string proper;
} Word;
class TextSpilt {
public:
  void init_ICTCAL(void);
  std::string ICTspilt(const char * sinput, int property);
  void goodWordArticlePorperty(const std::string &rawtext,
      std::set<std::string> &stopwords, std::vector<Word> &words);
  void goodWordArticle(const std::string &rawtext,
      std::set<std::string> &stopwords,
      std::vector<std::string> &goodword);
private:
  std::string RegexReplace(std::string input);
  std::wstring StringToWide(std::string &sToMatch);
  std::string WidetoString(std::wstring &wsm);
};

#endif
