/*
 * ArticlesInfo.h
 *
 *  Created on: 2014年8月22日
 *      Author: Administrator
 */

#ifndef ARTICLESINFO_H_
#define ARTICLESINFO_H_
#include"WeiboWord.h"
#include<iostream>
#include<string>
#include<map>
class ClassificationInfo{
  public:
  //该类别下的词频统计
  std::map<std::string,WeiboWord>word_fre;

  //该类别下的文章总数
  int article_num;

  //词的总数
  int word_num;


  std::map<std::string,WeiboWord> *GetWrodFre(){
    return &this->word_fre;
  }
};



#endif /* ARTICLESINFO_H_ */
