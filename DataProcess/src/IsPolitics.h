/*
 * IsPolitic.h
 *
 *  Created on: 2014年8月12日
 *      Author: hogachen
 *  description：
 *      政治类话题提取
 */

#ifndef ISPOLITIC_H_
#define ISPOLITIC_H_
#include"ClassificationInfo.h"
#include"DBoperation.h"
#include"WeiboWord.h"
#include"Topic.h"
#include<vector>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include<iostream>
#include<string>
#include<fstream>

class IsPolitics{
  //所有的话题下的词
  std::map<std::string, WeiboWord> keyword_map;

  //从新闻网站上提取的特征词
  std::map<std::string, double> politic_word_map;


  ClassificationInfo other_classification;

  ClassificationInfo politic_classification;

public:
  //这个话题选取的词的个数
  int topic_word_size;
	std::map<std::string, WeiboWord> *GetKeyWordMap (){
	  return &this->keyword_map;
	}
	std::map<std::string,double > *GetPoliticWordMap(){
	  return &this->politic_word_map ;
	}
	void GetkeyMap();
	void AddKeyToMapWithProperty(WeiboWord &word);
  void SplitWeiboSplit(std::string &str, std::vector<WeiboWord> &result);
  void GetEveryWordInOneTopicByWordProperty(Topic &onetopic);
  void IsTopicPolitics(Topic &onetopic);
  void IsTopicPoliticsByBaye(Topic &onetopic);

  void InitIsPolitics();
  ClassificationInfo *GetOtherClassificationInfo() {
    return &this->other_classification;
  }

  ClassificationInfo *GetPoliticClassificationInfo() {
    return &this->politic_classification;
  }

  void ReadBayeModel(ClassificationInfo classification, std::string filename);
};

#endif /* ISPOLITIC_H_ */
