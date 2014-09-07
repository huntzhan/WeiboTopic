/*
 * TrainModel.h
 *
 *  Created on: 2014年8月22日
 *      Author: Administrator
 *  description：
 *      统计训练样本集中词的词频，对训练样本进行特征提取，（采用TFIDF，需要词频，词在类别下文章出现的次数，类别下的文章总数，需要维护分别两个词频map）
 */

#ifndef TRAINMODEL_H_
#define TRAINMODEL_H_
#include"WeiboWord.h"
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
typedef std::pair<std::string, WeiboWord > PAIR3;
class TrainModel{

public:
  //停词表
  std::set<std::string> stopwords;

  //其他类别的词频表
  ClassificationInfo other_classification;

  //政治类别的词频表
  ClassificationInfo politic_classification;


  ClassificationInfo *GetOtherClassificationInfo(){
    return &this->other_classification;
  }

  ClassificationInfo *GetPoliticClassificationInfo(){
      return &this->politic_classification;
  }
  //训练样本的主函数
  void TrainClassModel(std::string otherdir, std::string politicdir);

  //读取停用词表
  void Readstopwordset();

  //添加每一个词到类别的词频表中
  void AddWordToMap(std::list<std::string> &articlelist, ClassificationInfo &classinfo);

  //读取训练集
  void ReadArticle(std::list < std::string > &articlelist,
      std::list < std::string > &politic_article_list, std::string otherdir, std::string politicdir);
  //训练集分词和预处理函数
  void SplitWord(std::list < std::string > &articlelist,
      std::list < std::string > &politic_article_list);

};




#endif /* TRAINMODEL_H_ */
