/*
 * CooccurrenceWord.h
 *
 *  Created on: 2014年8月4日
 *      Author: hogachen
 *  description：
 *     计算共现度的类，存放的是一个词对应于其他词的共现度
 */



#ifndef COOCCURRENCEWORD_H_
#define COOCCURRENCEWORD_H_
#include<iostream>
#include<map>
#include<string>
class CooccurrenceWord{
public:
	std::map<std::string,double>word_cooccurrence;
	std::map<std::string,double> *GetWordCooccurrence(){
		return & word_cooccurrence;
	}
};
#endif
