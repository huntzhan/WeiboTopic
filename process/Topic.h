/*
 * Cluster.h
 *
 *  Created on: 2014年7月31日
 *      Author: hogachen
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

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
	vector<TopicWord> getsTopic(){
		return this->m_stopic;
	}
	void addTopicWord(TopicWord t_word){
		this->m_stopic.push_back(t_word);
	}
};

#endif /* CLUSTER_H_ */
