/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 */
#include"DBoperation.h"
#include "GetTopic.h"
//#include "DBdao.h"
//#include"TopicView.h"
#include "Cluster.h"
#include "DEBUG.h"
#include<iostream>
#include<time.h>
//#define TOPIC_WORD_NUM 100
#define DEBUG
#define TIME

int main() {
#ifdef TIME
	time_t startmain;
	startmain = time(NULL);
#endif
	std::string current_time = "2013-04-12 12:00:00"; //格式："2013-04-12 12:00:00"
	int timeslip = 360000; //当前从开始时间到多久结束

	std::string K_hours_time = "2013-04-12 23:00:00";
	int ktimeslip = 360000; //前K个小时到多久结束

	int hours = 6; //时间窗口个数

	int TOPIC_WORD_NUM = 10000; //生成的主题词（待聚类的词）个数

	int RAND_SIZE = 10000; //计算聚类时的阈值的抽样个数

	int BELONG_TOPIC_THROD = 2; //计算倒排索引的时候有多少个词出现在一条微博就算这条微博属于这个话题

	int NUM_OF_SUB_WORD = 3; //提取子话题时提取的单位词的个数

	int THROD_ADD = 10; //调节阈值的参数

	int OneTimeReadWeiboNum=1000;//一次性读取的微博数

	int TOPICMAPTHROD = 100000; //如果map中的词的个数超过某个阈值，就删除掉一些
	std::list<std::string> table;
	std::list<std::string>::iterator table_it;
	ConnPool *connpool=ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
	DBoperation dboper;

	dboper.DBinit("use test",connpool);
	dboper.ShowTable(table); //查询有多少表格
	table_it=table.begin();
	for(;table_it!=table.end();++table_it){
		std::cout<<*table_it<<std::endl;
	}

	dboper.SetTableName(table.front());

	long weibosize=dboper.GetTablecount();
	//初始化第一个表的数据
	dboper.DBTableInit((int)weibosize,OneTimeReadWeiboNum,0,table);



	GetTopic gettopic;
	gettopic.InitGetTopic(&dboper, TOPIC_WORD_NUM, hours,TOPICMAPTHROD);
	gettopic.GenTopicWordByFrequency();
//#ifdef DEBUG
//	std::cout << "Running the GetTopic module ..." << std::endl;
//#endif
	std::map<std::string, TopicWord>* topicwordmap;

	topicwordmap = gettopic.GetTopicWord();

	Cluster cluster(&dboper, topicwordmap);
	cluster.InitConfigure(RAND_SIZE, BELONG_TOPIC_THROD, THROD_ADD,(int)weibosize);
//
//#ifdef DEBUG
//	std::cout << "Running the SinglePass module ..." << std::endl;
//#endif
	cluster.Singlepass();
//
////	TopicView topicview(&dbdao, cluster.GetClusterList(),
////			cluster.GetCooccurrence());
////	topicview.InitTopicView(NUM_OF_SUB_WORD, BELONG_TOPIC_THROD);
//#ifdef TIME
//	time_t start6;
//	start6 = time(NULL);
//#endif
////	topicview.GenAllTopicView();
#ifdef TIME
	time_t ends6;
	ends6 = time(NULL);
//	std::cout << "计算子话题用时：" << difftime(ends6, start6) << std::endl;
	std::cout << "整个过程用时：" << difftime(ends6, startmain) << " 秒"<<std::endl;
#endif
//#ifdef DEBUG
//	std::cout << "Process successfully!" << std::endl;
//#endif
	return 0;
}
