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
#include<set>
using namespace std;

class TopicWord {

public:
//	double IDF;
	string m_sword;
	double m_dFrequency;
	std::set<std::string> word_to_weiboid_list;//词对应的微博列表

	std::set<std::string>* GetWordToWeiboidList() {
		return &this->word_to_weiboid_list;
	}

	TopicWord(string word, double frequency) {
		m_sword = word;
		m_dFrequency = frequency;
//		this->IDF = 1.0;
	}
	string GetTopicWord() {
		return m_sword;
	}
	double GetFrequency() {
		return this->m_dFrequency;
	}
	void SetFrequency(double frequency) {
		this->m_dFrequency = frequency;
	}
};

#endif /* TOPICWORDS_H_ */

