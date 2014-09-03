/*
 * Cluster.h
 *
 *  Created on: 2014年8月4日
 *      Author: hogachen
 *  description：
 *      根据Gettopic模块处理好特征词map进行共现度的计算和一趟聚类，生成话题，同时将属于话题的微博对应到话题
 */

#ifndef CLUSTER_H_
#define CLUSTER_H_
#include "IsPolitics.h"
#include "TopicView.h"
#include "CooccurrenceWord.h"
#include "DBoperation.h"
#include "DBoperation.h"
#include "OneWeibo.h"
#include "Topic.h"
#include "TopicWord.h"
#include<pthread.h>
#include<unistd.h>
#include <iostream>
#include <map>
#include<algorithm>
#include <string>

class Cluster{

public:
  //提取子话题时提取的单位词的个数
  int NUM_OF_SUB_WORD;
  //话题下观点提取代表性的微博数
  int TOPICVIEW_WEIBO_NUM;

  //当话题下的微博数小于这个数（10）时，该话题无效
  int MIN_TOPIC_MESSAGE_NUM;

	int weibosize;
	//每次批量获取的数据量（1000）
	int OneTimeReadWeiboNum;

	//有多少（3）个特征词出现在一条微博时将就将该微博加入这个话题
	int BELONG_TOPIC_THROD;

	//抽样生成聚类阈值时的抽样数（1000）
	int RAND_SIZE;
	int randsize;

	//额外调节阈值的参数
	double THROD_ADD;

	//一趟聚类的阈值（自动生成）
	double CLSTER_THROD;

	//最终生成的话题列表
	std::vector<Topic> clusterList;
	std::vector<Topic> clusterListTemp;
	//存放待聚类的词（10000个）两两之间的共现度
	std::map<std::string,CooccurrenceWord> co_ccur_matrix;

	//保持的数据库访问对象
	DBoperation *dboper;

	//指向GetTopic生成的主题词
	std::map<std::string,TopicWord> *topicword;

	std::map<std::string,CooccurrenceWord> * GetCooccurrence(){
		return & co_ccur_matrix;
	}

	void SetClusterThrod(double throd){
		this->CLSTER_THROD=throd;
	}

	Cluster(DBoperation *dboper, std::map<std::string,TopicWord> *topicword){
		this->dboper =dboper;
		this->topicword=topicword;
	}

	void InitConfigure(int randsize, int BELONG_TOPIC_THROD,
	    double THROD_ADD, int weibosize, int MIN_TOPIC_MESSAGE_NUM,
	     int TOPICVIEW_WEIBO_NUM, int NUM_OF_SUB_WORD){
		this->RAND_SIZE=randsize;
		this->weibosize=weibosize;
		this->BELONG_TOPIC_THROD=BELONG_TOPIC_THROD;
		this->THROD_ADD=THROD_ADD;
		this->MIN_TOPIC_MESSAGE_NUM=MIN_TOPIC_MESSAGE_NUM;
		this->TOPICVIEW_WEIBO_NUM=TOPICVIEW_WEIBO_NUM;
		this->NUM_OF_SUB_WORD=NUM_OF_SUB_WORD;
	}

	void CalConWithTime();
	std::vector<Topic> * GetClusterList(){
		return & clusterList;
	}
	//计算词与话题之间的聚类
	double Cal_Words_Topic_Distance(Topic &topic,TopicWord &t_word);

	//一趟聚类的主函数
	void Singlepass();

	//计算词之间的共现度
	void CalWordsCoccurrence();

	//将所有的话题信息和话题下的对应的微博插入数据库
  void InsterAllTopicToDatabase();
  //插入一个话题
	void InsertTopicToDatabase(Topic &one_topic);

	//筛选话题下对应的微博
	void ListEveryTopicWeiboId(Topic &one_topic);
	void ListAllTopicWeiboId();

	//根据话题下的微博数目进行排序
	void SortTopic();

	//抽样产生聚类阈值
	double GenClusterThrod();
	std::vector<int> GenRandomValue();


	//根据词是否在这条微博出现来建立词到微博ID的倒排索引
	void MatchWeiboIDToTopic();
	void EraseCo_ccur_matrix();
	void MergeClusterList();
};
#endif /* CLUSTER_H_ */
