/*
 * Cluster.h
 *
 *  Created on: 2014年8月12日
 *      Author: hogachen
 *  description：
 *      话题类，存放话题的关键词列表，话题对应微博ID列表，话题主要观点列表，话题下有多少条微博
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
  //话题是否属于政治类的,1是政治类的，0不是政治类的
  int isPolitic;

  //话题下的特征词
	list<TopicWord> m_stopic;

	//临时统计微博ID出现的次数
	std::map<std::string, double> topic_weibo_id;

	//微博ID对应该ID出现的次数
	std::vector<subword> weibo_id_list;

	//话题下的主要观点临时统计数据结构
	std::map<std::string, double> main_idea;

	//最终的主要观点
	std::list<subword> sub_word_list;

	//获取到的话题微博
	std::list<Weibo> topic_weibo;
	int topic_message_num;
	std::list<Weibo>  *GetTopicWeibo(){
	  return &this->topic_weibo;
	}
	std::list<subword>* GetSubWordList() {
		return &this->sub_word_list;
	}
	std::map<std::string, double>* GetMainIdea() {
		return &this->main_idea;
	}
	std::map<std::string, double>* GetTopicWeiboId() {
		return &this->topic_weibo_id;
	}
	std::vector<subword> *GetWeiboIdList() {
		return &this->weibo_id_list;
	}
	//生成一个簇（话题）时，先将该话题的第一个点加入簇中
	void TopicInit(TopicWord firstWord) {
		this->m_stopic.push_back(firstWord);
		this->isPolitic=0;
	}
	list<TopicWord> *GetsTopic() {
		return &this->m_stopic;
	}
	void addTopicWord(TopicWord t_word) {
		this->m_stopic.push_back(t_word);
	}
};

#endif /* CLUSTER_H_ */
