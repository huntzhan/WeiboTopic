/*
 * Cluster.h
 *
 *  Created on: 2014骞�7鏈�31鏃�
 *      Author: hogachen
 */

#ifndef TOPIC_H_
#define TOPIC_H_

#include"TopicWord.h"
#include"Weibo.h"
#include<iostream>
#include<vector>
#include<string>
#include<list>
#include<map>
using namespace std;
class subword {
public:
	std::string word;
	double fre;

	subword(std::string word, double fre) {
		this->word = word;
		this->fre = fre;
	}
};
class Topic {


public:
	vector<TopicWord> m_stopic;
	std::map<std::string, double> topic_weibo_id; //临时统计微博ID出现的次数
	std::vector<subword> weibo_id_list; //微博ID对应该ID出现的次数
	std::map<std::string, double> main_idea; //话题下的主要观点临时统计数据结构
	std::list<subword> sub_word_list; //最终的主要观点
	std::list<Weibo> topic_weibo; //获取到的话题微博
	int topic_message_num;
	std::list<subword>* GetSubWordList() {
		return &this->sub_word_list;
	}
	std::map<std::string, double>* GetMainIdea() {
		return &this->main_idea;
	}
	std::map<std::string, double>* GetTopicWeiboId() {
		return &this->topic_weibo_id;
	}
	std::vector<subword> * GetWeiboIdList() {
		return &this->weibo_id_list;
	}

	void TopicInit(TopicWord firstWord) {//这里可以这样吗？
		this->m_stopic.push_back(firstWord);
	}
	vector<TopicWord> *GetsTopic() {
		return &this->m_stopic;
	}
	void addTopicWord(TopicWord t_word) {
		this->m_stopic.push_back(t_word);
	}
};

#endif /* CLUSTER_H_ */
