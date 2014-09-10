/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 *  description：
 *      先生成一个数据库的操作类，生成特征词（特征词，主题词，关键词都是指话题生成时的词，只是在不同阶段叫法不一样）
 *      再计算特征词 的共现度，最后一趟聚类生成话题
 */
#include"TestModel.h"
#include"TopicViewAndPolitics.h"
#include"TrainModel.h"
#include"DBoperation.h"
#include "GetTopic.h"
#include "Cluster.h"
#include "DEBUG.h"
#include "coverage.h"
#include<iostream>
 #include<time.h>
#define DEBUG
#define TIME



int main(int argc, char * argv[]) {

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
	int BELONG_TOPIC_THROD = 2;

	//提取子话题时提取的单位词的个数
	int NUM_OF_SUB_WORD = 4;

	//调节阈值的参数
	int THROD_ADD = 5;

	//一次性读取的微博数
	int OneTimeReadWeiboNum=1000;

	//如果map中的词的个数超过某个阈值，就删除掉一些
	int TOPICMAPTHROD = 100000;

	//话题下的微博数小于这个数时该话题不存入数据库
	int MIN_TOPIC_MESSAGE_NUM=5;
	//std::cout<<"run 0"<<std::endl;

	//现在的策略是处理微博数大于50000就将索引阈值置为3，否则为2
    int	TOTAL_WEIBO_NUM=50000;
	//链接数据库的信息
	std::list<std::string> table;
	std::string database_name;
	std::string topic_table_name;
	std::string output_filename;
//从主函数输入参数
  if(argc!=3){
    std::cout<<"参数个数不对，请重新输入"<<std::endl;
    return 0;
  }else{
      //只保留第一个表
      string flag=".";
      std::string input(argv[2]);

      int position = input.find(flag);

      database_name=input.substr(0,position);
      topic_table_name=input.substr(position+1,input.length());
      table.push_back(topic_table_name);

	  output_filename.assign(argv[1]);
  }

  //开始链接数据库
	ConnPool *connpool=ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 10);
	DBoperation dboper;


//	CCoverage m_coverage(&dboper);


	//初始化要使用的数据库
	dboper.DBinit(database_name,topic_table_name, connpool);

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
	if(gettopic.total_weibo_num<TOTAL_WEIBO_NUM)
		BELONG_TOPIC_THROD=2;
	else
		BELONG_TOPIC_THROD=3;
	//一趟聚类模块
	std::map<std::string, TopicWord>* topicwordmap;
	topicwordmap = gettopic.GetTopicWord();
	Cluster cluster(&dboper, topicwordmap);
	cluster.InitConfigure(RAND_SIZE, BELONG_TOPIC_THROD, THROD_ADD,
	    (int)weibosize,MIN_TOPIC_MESSAGE_NUM,TOPICVIEW_WEIBO_NUM,NUM_OF_SUB_WORD);
	cluster.CalConWithTime();
	cluster.Singlepass();
	//删除第一次聚类的特征词，降低聚类阈值和索引词个数，二次聚类
//	cluster.EraseCo_ccur_matrix();

	//生成话题主要观点模块、话题插入数据库模块、判断话题是否为政治类话题模块
	TopicViewAndPolitics view_and_politics;
	view_and_politics.InitTopicViewAndPolitics(&dboper,&cluster.clusterList,NUM_OF_SUB_WORD,
      MIN_TOPIC_MESSAGE_NUM, TOPICVIEW_WEIBO_NUM,&cluster.co_ccur_matrix,output_filename);
	view_and_politics.InsterAllTopicToDatabase();



#ifdef TIME
	time_t ends6;
	ends6 = time(NULL);
	std::cout << "整个过程用时：" << difftime(ends6, startmain) << " 秒"<<std::endl;
#endif
	CCoverage coverage(&dboper);
	coverage.MatchTopicByJaccard(topic_table_name,"OneDayTopic_"+topic_table_name);
#ifdef TIME
	time_t ends7;
	ends7 = time(NULL);
	std::cout << "计算覆盖率用时：" << difftime(ends7, startmain) << " 秒"<<std::endl;
#endif
	return 0;
}
