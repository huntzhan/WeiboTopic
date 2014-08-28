#include "Textspilt.h"
void TextSpilt::init_ICTCAL(void) {
  if (!ICTCLAS_Init()) {
    printf("Init fails\n");
    return;
  } else {
#ifdef DEBUG
    printf("Init ok\n");
#endif
  }
  //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
  ICTCLAS_SetPOSmap(2);
  unsigned int nItems = ICTCLAS_ImportUserDictFile("userdict.txt",
      CODE_TYPE_UTF8);
  std::cout << "add " << nItems << " user word" << std::endl;
  ICTCLAS_SaveTheUsrDic(); //保存用户词典
  std::setlocale(LC_ALL, "zh_CN.UTF-8");
}

std::string TextSpilt::ICTspilt(const char * sinput, int property) {
  std::string result;
  int nRstLen = 0;
  unsigned int nPaLen;
  nPaLen = strlen(sinput); // 需要分词的长度
  if (nPaLen < 2) {
    result = "";
    return result;
  }
  char* sRst = (char *) malloc(1024); //建议长度为字符串长度的倍。
  if (sRst == NULL)
    return NULL;
  nRstLen = ICTCLAS_ParagraphProcess(sinput, nPaLen, sRst, CODE_TYPE_UTF8,
      property);  //字符串处理
  result = sRst;
  free(sRst);
  sRst = NULL;
  return result;
}

/*************************************************************************************
 * 正则表达式去噪
 * input 需要去噪的句子
 * output 已经去砸的句子
 *  1 表情  如 [高兴]                     regex：-----------------   \\[([^x00-xff]*)\\]
 *  2 @微博 名         @traa @傅fuuuu @记得我叫青蛙忘了我叫俞映庭:  regex  -------- /?/?\@([^ ]*)[:]|/?/?\@([^ ]*)[ ]   以空格和:结尾
 *  3 超链接    http                   regex -----------------[a-zA-z]+://[^s]*
 *  3 一些特殊的字符              ~@#￥%%……&……                   regex ----------------[()——_*$#]*|【动画】|〜
 */

std::string TextSpilt::RegexReplace(std::string input) {
  std::string output;
  std::wstring put;
  if (input.empty()) {
    std::string result = "";
    return result;
  }
  //去掉表情
  input.append(" ");
  boost::wregex reg(
      L"\\[([^x00-xff]*)\\]|/?/?\@([^ ]*)[:]|/?/?\@([^ ]*)[ ]|[a-zA-z]+://[^s]*|",
      boost::regex::perl);
  put = boost::regex_replace(StringToWide(input), reg, std::wstring(L""));
  input = WidetoString(put);
  boost::wregex reg1(L"[^ 0-9a-zA-Z\u4e00-\u9fa5]*", boost::regex::perl);
  put = boost::regex_replace(StringToWide(input), reg1, std::wstring(L""));
  input = WidetoString(put);

  //去掉@人名转发     思路去输掉空格                  @非空格+空格
//  boost::wregex reg2(L"/?/?\@([^ ]*)[ ]", boost::regex::perl);
  //匹配非数字英文中文等特殊符号
  return input;
}

/*************************************************************************************
 * 窄字符转换成宽字符
 */
std::wstring TextSpilt::StringToWide(std::string &sToMatch) {
//  std::setlocale(LC_ALL,"zh_CN.UTF-8");
  int iWLen = 1024;
  wchar_t lpwsz[iWLen];
  mbstowcs(lpwsz, sToMatch.c_str(), iWLen - 2);
  lpwsz[iWLen - 2] = '\0';
  std::wstring wsToMatch(lpwsz);
  //std::setlocale(LC_ALL,"");
  return wsToMatch;
}
/*************************************************************************************
 * 宽字符转换成窄字符
 */
std::string TextSpilt::WidetoString(std::wstring &wsm) {
  std::string sToMatch;

  int iLen = 1024;
  char lpsz[iLen];
  wcstombs(lpsz, wsm.c_str(), iLen - 2); // 转换。（没有结束符）
  lpsz[iLen - 2] = '\0';
  sToMatch = lpsz;
  //  std::setlocale(LC_ALL,"");
  return sToMatch;
}

void TextSpilt::goodWordArticlePorperty(const std::string &rawtext,
                                        std::set<std::string> &stopwords, 
                                        std::vector<Word> &words) {
  std::vector<std::string> goodWordstemp;
  std::vector<std::string> goodWordstempSub;
  std::wstring wresult;
  if (stopwords.empty()) {
    std::cout << "input goodWordsinPieceArticle error" << std::endl;
    return;
  }
  std::string temp = RegexReplace(rawtext);  //先正则表达式

  std::string result = ICTspilt(temp.c_str(), 1);
  if (result == "") {
    Word word;
    word.word = "";
    word.proper = "";
    words.push_back(word);
    return;
  }
  boost::trim(result);
  //  wresult=StringToWide(result);
  boost::split(goodWordstemp, result, boost::is_any_of(" "));  //分割

  std::vector<std::string>::iterator it_good = goodWordstemp.begin();
  std::vector<std::string>::iterator end_good = goodWordstemp.end();
  std::string word, property;
  std::wstring temp2;
  for (; it_good < end_good; ++it_good) {
    //去掉一些特殊符号
    // boost::wregex reg3(L"[^\/0-9a-zA-Z\u4e00-\u9fa5]*|", boost::regex::perl);
    //  std::wstring result=boost::regex_replace(*it_good,reg3,std::wstring(L""));
    boost::split(goodWordstempSub, *it_good, boost::is_any_of("/"));   //分割
    if (goodWordstempSub.size() == 2) {
      word = goodWordstempSub[0];
      property = goodWordstempSub[1];
      if (!stopwords.count(word) && !word.empty()) {
        Word word_struct;
        word_struct.word = word;
        word_struct.proper = property;
        words.push_back(word_struct);
      }
    }

  }
  if (words.size() < 1) {
    Word word;
    word.word = "";
    word.proper = "";
    words.push_back(word);
  }

}

void TextSpilt::goodWordArticle(const std::string &rawtext,
    std::set<std::string> &stopwords, std::vector<std::string> &goodword) {

  std::vector<std::string> goodWordstemp;
  if (stopwords.empty()) {
    std::cout << "input goodWordsinPieceArticle error" << std::endl;
    return;
  }
  const std::string temp = RegexReplace(rawtext);  //先正则表达式
  std::string result = ICTspilt(temp.c_str(), 0);
  if (result == "") {
    goodword.push_back(result);
    return;
  }
  boost::trim(result);
  std::wstring wresult = StringToWide(result);
  boost::split(goodWordstemp, result, boost::is_any_of(" "));  //分割
  for (std::vector<std::string>::iterator it = goodWordstemp.begin();
      it != goodWordstemp.end(); it++) {
    std::string temp = *it;
    //     boost::trim(temp);
    if (!stopwords.count(temp) && !temp.empty()) {
      goodword.push_back(temp);
    }
  }
  if (goodword.size() < 1) {
    goodword.push_back("");
  }

}
