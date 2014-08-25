/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 *  description：
 *      先生成一个数据库的操作类，生成特征词（特征词，主题词，关键词都是指话题生成时的词，只是在不同阶段叫法不一样）
 *      再计算特征词 的共现度，最后一趟聚类生成话题
 */
#include"TopicViewAndPolitics.h"
#include"TrainModel.h"
#include"DBoperation.h"
#include "GetTopic.h"
#include "Cluster.h"
#include "DEBUG.h"
#include<iostream>
#include<time.h>
#define DEBUG
#define TIME

int main() {
#ifdef TIME
	time_t startmain;
	startmain = time(NULL);
#endif
	//话题下观点提取代表性的微博数
	int TOPICVIEW_WEIBO_NUM=50;
  //生成的主题词（待聚类的词）个数
	int TOPIC_WORD_NUM = 10000;

	//计算聚类时的阈值的抽样个数
	int RAND_SIZE = 10000;

	//计算倒排索引的时候有多少个词出现在一条微博就算这条微博属于这个话题
	int BELONG_TOPIC_THROD = 3;

	//提取子话题时提取的单位词的个数
	int NUM_OF_SUB_WORD = 10;

	//调节阈值的参数
	int THROD_ADD = 10;

	//一次性读取的微博数
	int OneTimeReadWeiboNum=1000;

	//如果map中的词的个数超过某个阈值，就删除掉一些
	int TOPICMAPTHROD = 100000;

	//话题下的微博数小于这个数时该话题不存入数据库
	int MIN_TOPIC_MESSAGE_NUM=10;

	ConnPool *connpool=ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
	DBoperation dboper;

	//初始化要使用的数据库
	dboper.DBinit("use split",connpool);

	//查询该数据库有多少表
	std::list<std::string> table;
	std::list<std::string>::iterator table_it;
	dboper.ShowTable(table);
	table_it=table.begin();
	for(;table_it!=table.end();++table_it){
		std::cout<<*table_it<<std::endl;
	}

	//设置要访问的表
	dboper.SetTableName(table.front());

	//查询表中数据量
	long weibosize=dboper.GetTablecount();

	//初始化第一个表的数据
	dboper.DBTableInit((int)weibosize,OneTimeReadWeiboNum,0,table);


	//特征词提取模块
	GetTopic gettopic;
	gettopic.InitGetTopic(&dboper, TOPIC_WORD_NUM,TOPICMAPTHROD);
	gettopic.GenTopicWordByFrequency();

	//一趟聚类模块
	std::map<std::string, TopicWord>* topicwordmap;
	topicwordmap = gettopic.GetTopicWord();
	Cluster cluster(&dboper, topicwordmap);
	cluster.InitConfigure(RAND_SIZE, BELONG_TOPIC_THROD, THROD_ADD,
	    (int)weibosize,MIN_TOPIC_MESSAGE_NUM,TOPICVIEW_WEIBO_NUM,NUM_OF_SUB_WORD);
	cluster.Singlepass();


	//生成话题主要观点模块、话题插入数据库模块、判断话题是否为政治类话题模块
	TopicViewAndPolitics view_and_politics;
	view_and_politics.InitTopicViewAndPolitics(&dboper,&cluster.clusterList,NUM_OF_SUB_WORD,
      MIN_TOPIC_MESSAGE_NUM, TOPICVIEW_WEIBO_NUM);
	view_and_politics.InsterAllTopicToDatabase();




#ifdef TIME
	time_t ends6;
	ends6 = time(NULL);
	std::cout << "整个过程用时：" << difftime(ends6, startmain) << " 秒"<<std::endl;
#endif
//  TrainModel tm;
//  tm.TrainClassModel();
	return 0;
}
