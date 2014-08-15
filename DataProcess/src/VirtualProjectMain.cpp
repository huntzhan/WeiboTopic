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
#define TIME
class Configure{
public:
};
int main(){
#ifdef TIME
	time_t startmain;
	startmain=time(NULL);
#endif
	std::string current_time="2013-04-12 12:00:00"; //格式："2013-04-12 12:00:00"
	int timeslip=360000;								//当前从开始时间到多久结束

	std::string K_hours_time="2013-04-12 23:00:00";
	int ktimeslip=360000;							//前K个小时到多久结束

	int hours=6;									//时间窗口个数

	int TOPIC_WORD_NUM=1000 ;     					//生成的主题词（待聚类的词）个数

	int RAND_SIZE=1000;								//计算聚类时的阈值的抽样个数

	int BELONG_TOPIC_THROD =2;						//计算倒排索引的时候有多少个词出现在一条微博就算这条微博属于这个话题

	int NUM_OF_SUB_WORD=3;							//提取子话题时提取的单位词的个数

	int THROD_ADD=3;								//调节阈值的参数

	int TOPICMAPTHROD =1000000;						//如果map中的词的个数超过某个阈值，就删除掉一些
	DBdao dbdao;
	dbdao.DBdaoInit(current_time,timeslip,K_hours_time,ktimeslip,hours);
	std::cout<<"stop words size is: "<<dbdao.stop_word_set.size()<<std::endl;
	std::cout<<"The current weibo list size is: "<<dbdao.weibo_id_list.size()<<std::endl;	
	std::cout<<"The K hours weibo list size is: "<<dbdao.k_hours_weibo_id_list.size()<<std::endl;
//	std::list<std::string>::iterator list_it =dbdao.weibo_id_list.begin();
//	for (; list_it != dbdao.weibo_id_list.end(); ++list_it) {
//		std::string weiboid =*list_it;
//		Weibo weibo;
//		dbdao.GetEveryWeiboWithProperty(weiboid, weibo);
//		std::vector<Word>::iterator itt=weibo.GetContentWithProperty()->begin();
//		for(;itt!=weibo.GetContentWithProperty()->end();++itt){
//			std::cout << itt->word<<"		"<<itt->proper<< std::endl;
//		}
//	}
//#ifdef TIME
//	time_t start3;
//	start3=time(NULL);
//#endif
//	Weibo weibo;
//	for (; list_it != dbdao.weibo_id_list.end(); ++list_it) {
//		std::string weiboid = *list_it;
//
//		dbdao.GetEveryWeiboWithProperty(weiboid, weibo);
////		dbdao.GetOriginalWeibo(weiboid);
////		dbdao.GetEveryWeiboFromDatabase(weiboid,weibo);
////		std::vector<std::string>::iterator itt =
////				weibo.GetWords()->begin();
////		for (; itt != weibo.GetWords()->end(); ++itt) {
////			std::cout <<*itt << std::endl;
////		}
//	}
//
//#ifdef TIME
//	time_t ends3;
//	ends3=time(NULL);
//	std::cout<<"读取5000条微博用时："<<difftime(ends3,start3)<<std::endl;
//#endif

	GetTopic gettopic;
	gettopic.InitGetTopic(&dbdao,TOPIC_WORD_NUM,hours,dbdao.weibo_id_list.size(),TOPICMAPTHROD);


//	gettopic.GenTopicWord();
	gettopic.GenTopicWordByFrequency();
#ifdef DEBUG
	std::cout<<"Running the GetTopic module ..."<<std::endl;
#endif
	std::map<std::string,TopicWord>* topicwordmap;

	topicwordmap=gettopic.GetTopicWord();

	Cluster cluster(&dbdao,topicwordmap);
	cluster.InitConfigure(RAND_SIZE,BELONG_TOPIC_THROD,THROD_ADD);



#ifdef DEBUG
	std::cout<<"Running the SinglePass module ..."<<std::endl;
#endif
	cluster.Singlepass();

	TopicView topicview(&dbdao,cluster.GetClusterList(),cluster.GetCooccurrence());
	topicview.InitTopicView(NUM_OF_SUB_WORD,BELONG_TOPIC_THROD);
#ifdef TIME
	time_t start6;
	start6=time(NULL);
#endif
	topicview.GenAllTopicView();
#ifdef TIME
	time_t ends6;
	ends6=time(NULL);
	std::cout<<"计算子话题用时："<<difftime(ends6,start6)<<std::endl;
	std::cout<<"整个过程题用时："<<difftime(ends6,startmain)<<std::endl;
#endif
#ifdef DEBUG
	std::cout<<"Process successfully!"<<std::endl;
#endif
	return 0;
}
