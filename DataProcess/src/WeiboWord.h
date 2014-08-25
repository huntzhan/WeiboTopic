/*
 * WweiboWord.h
 *
 *  Created on: 2014年8月21日
 *      Author: Administrator
 */

#ifndef WWEIBOWORD_H_
#define WWEIBOWORD_H_
#include<iostream>
#include<string>

class WeiboWord{
public :
  //词本身
  std::string wordstr;

  //词性
  std::string wordpro;

  //词频
  double wordfre;

  //用于在计算TFIDF时判断是否在某一篇文章中出现过
  bool isoccured;

  //文档频率
  double docfre;

  //TFIDF值
  double tfidf;
  void SetFrequency(double fre){
    this->wordfre=fre;
  }

  void SetTfidf(double  tfidf){
    this->tfidf=tfidf;
  }
  WeiboWord(std::string wordstr, std::string wrodpro, double wordfre){
    this->wordstr=wordstr;
    this->wordfre=wordfre;
    this->wordpro=wordpro;
    isoccured=true;
    docfre=1.0;
    tfidf=0.0;
  }
};



#endif /* WWEIBOWORD_H_ */


