/*
 * TestModel.h
 *
 *  Created on: 2014年8月25日
 *      Author: Administrator
 */

#ifndef TESTMODEL_H_
#define TESTMODEL_H_
#include"DBoperation.h"
#include"WeiboWord.h"
#include"Topic.h"
#include"ClassificationInfo.h"
#include "ICTCLAS50.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<list>
#include<map>
#include<fstream>
class FilenameAndContent{
public:
  std::string filename;
  std::string content;
  int ispolitic;
};
class Article{
public:
  std::map<std::string, double> all_word;
  int ispolitic;
  int articlewordnum;
  std::string filename;
  int orignclass;
  void setpolitic(){
    this->ispolitic=0;
    this->articlewordnum=0;
  }
};
class TestModel{
public:
  //停词表
   std::set<std::string> stopwords;
   ClassificationInfo other_classification;


   ClassificationInfo politic_classification;
   void ReadTest();
   void InitIsPolitics();
   void CalArticleBayePro(Article &one_article);
   void Readstopwordset();
   void GetWordInOneArticle(std::string content, Article &one_article);
   void CalArticleBayePro(int &politics_article_num,int &belong_to_politic_num,Article &one_article,std::list<std::string>&goodfilename);
   void ProcessTestFile(std::list < FilenameAndContent> &politic_article_list);
   void ReadBayeModel(ClassificationInfo &classification, std::string filename);
   std::string intTostring(int a);
   void init_ICTCAL(void);
   std::string WidetoString2(std::wstring &wsm);
   std::wstring StringToWide2(std::string &sToMatch);
   std::string RegexReplace(std::string input);
   std::string ICTspilt(const char * sinput,int property);
   void SaveUsefulText(std::list<std::string>&filenamelist);
};




#endif /* TESTMODEL_H_ */
