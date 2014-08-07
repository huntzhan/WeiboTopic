/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
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
//	std::string filename="stopwords.txt";
	dbdao.DBdaoInit();
	std::cout<<"stop words size is: "<<dbdao.stop_word_set.size()<<std::endl;
	std::cout<<"The current weibo list size is: "<<dbdao.weibo_id_list.size()<<std::endl;	
	std::cout<<"The K hours weibo list size is: "<<dbdao.k_hours_weibo_id_list.size()<<std::endl;

	GetTopic gettopic(dbdao,TOPIC_WORD_NUM,K_WINDOW);


	gettopic.GenTopicWord();
#ifdef DEBUG
	std::cout<<"in main debug"<<std::endl;
#endif
	std::map<std::string,double>* topicwordmap;

	topicwordmap=gettopic.GetTopicWord();
	Cluster cluster(dbdao,topicwordmap);

	cluster.Singlepass();

#ifdef DEBUG
	std::cout<<"in main debug2"<<std::endl;
#endif
	return 0;
}
