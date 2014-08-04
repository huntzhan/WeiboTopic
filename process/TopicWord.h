/*
 * topicWords.h
 *
 *  Created on: 2014年7月31日
 *      Author: hogachen
 *  Discription:
 *  	主题词类，包括单词和这个主题词的权值
 */

#ifndef TOPICWORDS_H_
#define TOPICWORDS_H_

#include<iostream>
#include<string>
using namespace std;
class TopicWord{
	string m_sword;
	double m_dFrequency;
public:
	TopicWord(string word,double frequency){
		m_sword=word;
		m_dFrequency=frequency;
	}
	string * GetTopicWord(){
		return & m_sword;
	}
	double GetFrequency(){
		return this->m_dFrequency;
	}
};

#endif /* TOPICWORDS_H_ */

