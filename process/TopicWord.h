/*
 * topicWords.h
 *
 *  Created on: 2014��7��31��
 *      Author: hogachen
 *  Discription:
 *  	������࣬�������ʺ��������ʵ�Ȩֵ
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

