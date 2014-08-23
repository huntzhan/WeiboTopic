/*
 * TrainModel.h
 *
 *  Created on: 2014年8月22日
 *      Author: Administrator
 *  description：
 *      对训练样本进行特征提取，（采用TFIDF，需要词频，词在类别下文章出现的次数，类别下的文章总数，需要维护分别两个词频map），接着对微博文本特征提取，计算后验概率
 *      （需要）
 */

#ifndef TRAINMODEL_H_
#define TRAINMODEL_H_
#include"ClassificationInfo.h"
#include "ICTCLAS50.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include<iostream>
#include<string>
#include<list>
#include<map>
#include<fstream>
class TrainModel{

public:
  ClassificationInfo other_classification;


  ClassificationInfo politic_classification;

  ClassificationInfo *GetOtherClassificationInfo(){
    return &this->other_classification;
  }

  ClassificationInfo *GetPoliticClassificationInfo(){
      return &this->politic_classification;
  }
  void TrainClassModel();
  void ReadArticle(std::list < std::string > &articlelist,
      std::list < std::string > &politic_article_list);

  void SplitWord(std::list < std::string > &articlelist,
      std::list < std::string > &politic_article_list);
};




#endif /* TRAINMODEL_H_ */
