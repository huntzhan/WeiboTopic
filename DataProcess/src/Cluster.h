/*
 * Cluster.h
 *
 *  Created on: 2014年8月4日
 *      Author: hogachen
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_
#include"DBdao.h"
#include "Topic.h"
#include "TopicWord.h"
#include"CooccurrenceWord.h"
#include<iostream>
#include<map>
#include<string>

class Cluster{

public:
	int BELONG_TOPIC_THROD;
	int randsize;
	int RAND_SIZE;
	double THROD_ADD;//额外调节阈值的参数
	double CLSTER_THROD;
	std::vector<Topic> clusterList;//最终生成的话题列表
	std::map<std::string,CooccurrenceWord> co_ccur_matrix;
	DBdao *dbdao;
	map<string,TopicWord> *topicword;//指向GetTopic生成的主题词
	std::map<std::string,CooccurrenceWord> * GetCooccurrence(){
		return & co_ccur_matrix;
	}
	void SetClusterThrod(double throd){
		this->CLSTER_THROD=throd;
	}
	Cluster(DBdao  *dbdao,std::map<std::string,TopicWord> *topicword){

		this->dbdao=dbdao;
		this->topicword=topicword;
	}
	void InitConfigure(int randsize,int BELONG_TOPIC_THROD,double THROD_ADD){
		this->RAND_SIZE=randsize;
		this->BELONG_TOPIC_THROD=BELONG_TOPIC_THROD;
		this->THROD_ADD=THROD_ADD;
	}
	std::vector<Topic> * GetClusterList(){
		return & clusterList;
	}
	double Cal_Words_Topic_Distance(Topic &topic,TopicWord &t_word);
	void Singlepass();
	void CalWordsCooccurrence();
	void InsertTopicToDatabase(Topic &one_topic);
	void ListEveryTopicWeiboId(Topic &one_topic);
	void ListAllTopicWeiboId();
	void SortTopic();

	double GenClusterThrod();
	std::vector<int> GenRandomValue();
};
#endif /* CLUSTER_H_ */
