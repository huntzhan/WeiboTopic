/*
 * Cluster.h
 *
 *  Created on: 2014Äê7ÔÂ31ÈÕ
 *      Author: hogachen
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include"TopicWord.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;
class Cluster{
	vector<TopicWord> m_stopic;

public:
	Cluster(TopicWord firstWord){
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
