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
	std::map<std::string ,double> topic_weibo_id;
public:
	std::map<std::string ,double>* GetTopicWeiboId(){
		return &this->topic_weibo_id;
	}
	Topic(TopicWord &firstWord){//这里可以这样吗？
		m_stopic.push_back(firstWord);
	}
	vector<TopicWord> *GetsTopic(){
		return &this->m_stopic;
	}
	void addTopicWord(TopicWord t_word){
		this->m_stopic.push_back(t_word);
	}
};

#endif /* CLUSTER_H_ */
