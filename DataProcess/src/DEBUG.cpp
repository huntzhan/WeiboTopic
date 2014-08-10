/*
 * DEBUG.cpp
 *
 *  Created on: 2014年8月6日
 *      Author: hogachen
 */

#include"DEBUG.h"
#include"Weibo.h"
#define BELONT_TOPIC_THROD 1.0
void printVectors(std::list<std::string>&mylist){
	std::list<std::string>::iterator it = mylist.begin();
	int count=0;
	for(;it!=mylist.end();++it){
		count++;
		if(count>10)break;
		std::cout<<*it<<std::endl;
	}
}

void printMaps(std::map<std::string,TopicWord> &mymap){
	std::cout<<mymap.size()<<std::endl;
	std::map<std::string,TopicWord>::iterator it=mymap.begin();
	int count=0;
	for(;it!=mymap.end();++it){
		count++;
		if(count>10000000)break;
		std::cout<<it->first<<"		"<<it->second.m_dFrequency<<"		"<<it->second.GetWordToWeiboidList()->size()<<std::endl;
//		if(it->second.m_dFrequency<0||it->second.m_dFrequency>MAX){
//
//		}
	}
}
void printTopic(std::vector<Topic>&clusterlist,DBdao *dbdao){
	std::vector<Topic>::iterator clit=clusterlist.begin();
	int ti=0;
	for(;clit!=clusterlist.end();++clit){
		++ti;
//		Topic topic = *clit;
		std::vector<TopicWord>*topic_vec=clit->GetsTopic();
		std::vector<TopicWord>::iterator it=topic_vec->begin();
		std::cout<<std::endl<<std::endl;
		std::cout<<"话题 "<<ti<<"如下： "<<std::endl;
		for(;it!=topic_vec->end();++it){
			std::cout<<it->GetTopicWord()<<"	";//<<it->GetFrequency()<<"	";
		}
		std::cout<<std::endl;
			std::map<std::string,double>*topic_weibo_id_map=clit->GetTopicWeiboId();
			double weiboid_num=0.0;
			std::string weiboid;
			std::map<std::string,double>::iterator topic_weibo_id_map_it=topic_weibo_id_map->begin();
			for(;topic_weibo_id_map_it!=topic_weibo_id_map->end();++topic_weibo_id_map_it){
				weiboid=topic_weibo_id_map_it->first;
				weiboid_num=topic_weibo_id_map_it->second;
				std::cout<<"话题词在本微博出现次数："<<weiboid_num<<std::endl;
				if(weiboid_num>=BELONT_TOPIC_THROD){
					//这里应该插入数据库
					//InsertOneTopicToDatabase(one_topic);
					Weibo oneweibo;
					dbdao->GetEveryWeiboFromDatabase(weiboid,oneweibo);
					vector<string>::iterator w_c_it= oneweibo.GetWords()->begin();
					std::cout<<"topic weibo ：";
					for(;w_c_it!=oneweibo.GetWords()->end();++w_c_it)
						std::cout<<*w_c_it<<" ";
					std::cout<<std::endl;
				}
			}
	}
}

void printMatrix(std::map<std::string,CooccurrenceWord> &co_ccur_matrix){
	std::map<std::string,CooccurrenceWord>::iterator it = co_ccur_matrix.begin();
	for(;it!=co_ccur_matrix.end();++it){
		std::cout<<it->first<<"		word map size is : "<<it->second.word_cooccurrence.size()<<std::endl;
		std::map<std::string,double>::iterator it2= it->second.GetWordCooccurrence()->begin();
		for(;it2!=it->second.GetWordCooccurrence()->end();++it2){
			std::cout<<it2->first<<"		"<<it2->second;
		}
		std::cout<<std::endl;
	}
}
