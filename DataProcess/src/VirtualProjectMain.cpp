/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 */


#include "GetTopic.h"
#include "DBdao.h"
#include"TopicView.h"
#include "Cluster.h"
#include "DEBUG.h"
#include<iostream>
//#define TOPIC_WORD_NUM 100
#define DEBUG
class Configure{
public:
};
int main(){
	std::string current_time="2013-04-12 12:00:00"; //格式："2013-04-12 12:00:00"
	int timeslip=3600;								//当前从开始时间到多久结束

	std::string K_hours_time="2013-04-12 23:00:00";
	int ktimeslip=360000;							//前K个小时到多久结束

	int hours=6;									//时间窗口个数

	int TOPIC_WORD_NUM=100 ;     					//生成的主题词（待聚类的词）个数

	int RAND_SIZE=100;								//计算聚类时的阈值的抽样个数

	int  BELONG_TOPIC_THROD =2;						//计算倒排索引的时候有多少个词出现在一条微博就算这条微博属于这个话题

	int NUM_OF_SUB_WORD=3;							//提取子话题时提取的单位词的个数
	DBdao dbdao;
	dbdao.DBdaoInit(current_time,timeslip,K_hour_time,ktimeslip,hours);
	std::cout<<"stop words size is: "<<dbdao.stop_word_set.size()<<std::endl;
	std::cout<<"The current weibo list size is: "<<dbdao.weibo_id_list.size()<<std::endl;	
	std::cout<<"The K hours weibo list size is: "<<dbdao.k_hours_weibo_id_list.size()<<std::endl;

	GetTopic gettopic(&dbdao,TOPIC_WORD_NUM,hours);


//	gettopic.GenTopicWord();
	gettopic.GenTopicWordByFrequency();
#ifdef DEBUG
	std::cout<<"Running the GetTopic module ..."<<std::endl;
#endif
	std::map<std::string,TopicWord>* topicwordmap;

	topicwordmap=gettopic.GetTopicWord();

	Cluster cluster(&dbdao,topicwordmap);
	cluster.InitConfigure(RAND_SIZE,BELONG_TOPIC_THROD);



#ifdef DEBUG
	std::cout<<"Running the SinglePass module ..."<<std::endl;
#endif
	cluster.Singlepass();

	TopicView topicview(&dbdao,cluster.GetClusterList(),cluster.GetCooccurrence());
	topicview.InitTopicView();
	topicview.GenAllTopicView(NUM_OF_SUB_WORD);
#ifdef DEBUG
	std::cout<<"Process successfully!"<<std::endl;
#endif
	return 0;
}
