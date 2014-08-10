/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 */


#include "GetTopic.h"
#include "DBdao.h"
#include "Cluster.h"
#include "DEBUG.h"
#include<iostream>
#define TOPIC_WORD_NUM 100
#define K_WINDOW 7
#define DEBUG
int main(){


	DBdao dbdao;
	dbdao.DBdaoInit();
	std::cout<<"stop words size is: "<<dbdao.stop_word_set.size()<<std::endl;
	std::cout<<"The current weibo list size is: "<<dbdao.weibo_id_list.size()<<std::endl;	
	std::cout<<"The K hours weibo list size is: "<<dbdao.k_hours_weibo_id_list.size()<<std::endl;

	GetTopic gettopic(&dbdao,TOPIC_WORD_NUM,K_WINDOW);


	gettopic.GenTopicWord();
#ifdef DEBUG
	std::cout<<"Running the GetTopic module ..."<<std::endl;
#endif
	std::map<std::string,TopicWord>* topicwordmap;

	topicwordmap=gettopic.GetTopicWord();

	Cluster cluster(&dbdao,topicwordmap);



#ifdef DEBUG
	std::cout<<"Running the SinglePass module ..."<<std::endl;
#endif
	cluster.Singlepass();

#ifdef DEBUG
	std::cout<<"Process successfully!"<<std::endl;
#endif
	return 0;
}
