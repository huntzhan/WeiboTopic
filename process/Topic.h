/*
 * Cluster.h
 *
 *  Created on: 2014骞�7鏈�31鏃�
 *      Author: hogachen
 */

#ifndef TOPIC_H_
#define TOPIC_H_

#include"TopicWord.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;
class Topic{
	vector<TopicWord> m_stopic;

public:
	Topic(TopicWord firstWord){
		m_stopic.push_back(firstWord);
	}
	vector<TopicWord> GetsTopic(){
		return this->m_stopic;
	}
	void addTopicWord(TopicWord t_word){
		this->m_stopic.push_back(t_word);
	}
};

#endif /* CLUSTER_H_ */
