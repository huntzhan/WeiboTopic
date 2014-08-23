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

  void SetFrequency(double fre){
    this->wordfre=fre;
  }
  WeiboWord(std::string wordstr, std::string wrodpro, double wordfre){
    this->wordstr=wordstr;
    this->wordfre=wordfre;
    this->wordpro=wordpro;
  }
};



#endif /* WWEIBOWORD_H_ */
