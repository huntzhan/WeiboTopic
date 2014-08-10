/*
 * cluster.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: hogachen
 */

#include"DEBUG.h"
#include"Cluster.h"
#include "CooccurrenceWord.h"
#include"TopicWord.h"
#include"Topic.h"
#include"DBdao.h"
#include<iostream>
#include<string>
using namespace std;
#define THROD 3.0
#define MINVALUE -1
#define BELONT_TOPIC_THROD 2.0
//#define DEBUG_CLUSTER1
//#define CALDIS
#define PRINTTOPIC2
typedef map<string,TopicWord> MAP;

void Cluster::CalWordsCooccurrence(){

	list<string>::iterator weiboit= this->dbdao->weibo_id_list.begin();
	for(;weiboit!= this->dbdao->weibo_id_list.end();++weiboit){
		string weiboID = *weiboit;
		Weibo oneweibo;
		this->dbdao->GetEveryWeiboFromDatabase(weiboID,oneweibo);
		std::vector<std::string>*vec = oneweibo.GetWords();

		std::vector<std::string>::iterator vec_it = vec->begin();
		std::vector<std::string>word_coou_oneweibo;
		for(;vec_it!=vec->end();++vec_it){
			std::string oneword = * vec_it;
			//std::cout<<*vec_it<<std::endl;
			if(this->topicword->count(oneword)){
				word_coou_oneweibo.push_back(oneword);
			}
		}

#ifdef DEBUG_CLUSTER
	std::cout<<"CalWordsCooccurrence finished2!"<<std::endl;
#endif
		for(int i=0;i<word_coou_oneweibo.size();++i){
			std::string firstword = word_coou_oneweibo[i];
			std::map<std::string,CooccurrenceWord>::iterator it=co_ccur_matrix.find(firstword);
			CooccurrenceWord occurword;
//			if(it!=co_ccur_matrix.end()){
//				occurword=it->second;//这里是一个局部变量了
//			}else{
//				co_ccur_matrix.insert(make_pair(firstword,occurword));
//			}

			if(it==co_ccur_matrix.end()){
				CooccurrenceWord occurword;
				std::map<std::string,double>* map_wordmap=occurword.GetWordCooccurrence();
				std::map<std::string,double>::iterator find_it;
				for(int j = 0;j<word_coou_oneweibo.size();++j){
					std::string secondword=word_coou_oneweibo[j];
					if(firstword.compare(secondword)==0)
							continue;
					find_it=map_wordmap->find(secondword);
					if(find_it!=map_wordmap->end()){
						find_it->second=find_it->second+1;
					}else{
						map_wordmap->insert(make_pair(secondword,1.0));
//						find_it->second=1.0;
					}
				}
				co_ccur_matrix.insert(make_pair(firstword,occurword));
				continue;
			}
			std::map<std::string,double>* map_wordmap=it->second.GetWordCooccurrence();
			std::map<std::string,double>::iterator find_it=map_wordmap->begin();
			for(int j = 0;j<word_coou_oneweibo.size();++j){
				std::string secondword=word_coou_oneweibo[j];
				if(firstword.compare(secondword)==0)
					continue;
				find_it=map_wordmap->find(secondword);
				if(find_it!=map_wordmap->end()){
				find_it->second=find_it->second+1;
				}else{
					map_wordmap->insert(make_pair(secondword,1.0));
				}
			}
		}
	}
}

void Cluster::Singlepass(){
	this->CalWordsCooccurrence();

#ifdef DEBUG_CLUSTER1
	printMatrix(this->co_ccur_matrix);
#endif
//	vector<Topic> v_clusterList;

	MAP::iterator topic_w_it = this->topicword->begin();
//	TopicWord t_word(topic_w_it->first,topic_w_it->second);

	Topic topic(topic_w_it->second);
	this->clusterList.push_back(topic);
	topic_w_it++;
	for(;topic_w_it != this->topicword->end();++topic_w_it){

//		TopicWord t_words(topic_w_it->first,topic_w_it->second);

#ifdef DEBUG_CLUSTER
	std::cout<<"singlepass process!"<<std::endl;
#endif

		vector<Topic>::iterator vec_clu_it=this->clusterList.begin();
		double maxDistance=MINVALUE;
		vector<Topic>::iterator belong_clus_it=vec_clu_it;//这里应该是赋值还是指针？？？？
		for(;vec_clu_it != this->clusterList.end();++vec_clu_it){
//			Topic temptopic=;
			double words_distance=Cal_Words_Topic_Distance(*vec_clu_it,topic_w_it->second);
			if(maxDistance < words_distance){
				maxDistance = words_distance;
				belong_clus_it=vec_clu_it;
			}
		}
#ifdef CALDIS
	std::cout<<"CALDIS finish one!"<<std::endl;
#endif
		if(maxDistance < THROD){
			Topic newTopic(topic_w_it->second);
			this->clusterList.push_back(newTopic);//这里虽然newTopic是局部变量，但是由于会复制一个新的，有类的时候会动态调用拷贝构造函数
		}else{
			belong_clus_it->addTopicWord(topic_w_it->second);
		}
	}
	this->ListAllTopicWeiboId();
#ifdef PRINTTOPIC2
	printTopic(this->clusterList,this->dbdao);
#endif
}
double Cluster::Cal_Words_Topic_Distance(Topic &topic,TopicWord &topic_word){
#ifdef CALDIS
	std::cout<<"cal distance process!"<<std::endl;
#endif
	double topic_word_dis=0.0;
	std::string keyword = topic_word.GetTopicWord();
	std::map<std::string,double>* map_map;
	std::map<string,CooccurrenceWord>::iterator co_it=co_ccur_matrix.find(keyword);
	if(co_it!=co_ccur_matrix.end()){
//		CooccurrenceWord tempCooc=co_it->second;
		map_map=co_it->second.GetWordCooccurrence();
	}
	else {
		return 0.0;
	}
	vector<TopicWord>::iterator clu_it=topic.GetsTopic()->begin();//这里一定要指针？？为什么？
	std::string one_topic_word;
	std::map<std::string,double>::iterator second_it;
	for(;clu_it!=topic.GetsTopic()->end();++clu_it){
		one_topic_word= clu_it->GetTopicWord();
		second_it=map_map->find(one_topic_word);
		if(second_it!=map_map->end()){
			topic_word_dis+=second_it->second;
//			std::cout<<"coodis is ： "<<second_it->second<<std::endl;

		}
	}
//	std::cout<<topic_word_dis<<std::endl;
	return topic_word_dis;
}
void Cluster::ListAllTopicWeiboId(){
	int mycount=0;
	std::vector<Topic>::iterator clusterList_it = this->clusterList.begin();
	for(;clusterList_it!=this->clusterList.end();++clusterList_it){
		ListEveryTopicWeiboId(*clusterList_it);
//		std::cout<<mycount++<<std::endl;
//		InsertTopicToDatabase(*clusterList_it);
	}
}
void Cluster::ListEveryTopicWeiboId(Topic &one_topic){
	vector<TopicWord>::iterator topic_it= one_topic.GetsTopic()->begin();
	std::map<std::string,double>*topic_weibo_id_map=one_topic.GetTopicWeiboId();
	std::map<std::string,double>::iterator topic_weibo_id_map_it;

	for(;topic_it!= one_topic.GetsTopic()->end();++topic_it){
		std::set<std::string>::iterator topicword_weibolist_it = topic_it->GetWordToWeiboidList()->begin();
		for(;topicword_weibolist_it!=topic_it->GetWordToWeiboidList()->end();++topicword_weibolist_it){
			std::string weiboid=*topicword_weibolist_it;
			topic_weibo_id_map_it=topic_weibo_id_map->find(weiboid);
			if(topic_weibo_id_map_it!=topic_weibo_id_map->end()){
				topic_weibo_id_map_it->second=topic_weibo_id_map_it->second+1;
			}else{
				topic_weibo_id_map->insert(make_pair(weiboid,1.0));
			}
		}
	}
}

void Cluster::InsertTopicToDatabase(Topic &one_topic){
	std::map<std::string,double>*topic_weibo_id_map=one_topic.GetTopicWeiboId();
	double weiboid_num=0.0;
	std::string weiboid;
	std::map<std::string,double>::iterator topic_weibo_id_map_it;
	for(;topic_weibo_id_map_it!=topic_weibo_id_map->end();++topic_weibo_id_map_it){
		weiboid=topic_weibo_id_map_it->first;
		weiboid_num=topic_weibo_id_map_it->second;
		if(weiboid_num>BELONT_TOPIC_THROD){
			//这里应该插入数据库
			//InsertOneTopicToDatabase(one_topic);
			Weibo oneweibo;
			this->dbdao->GetEveryWeiboFromDatabase(weiboid,oneweibo);
			std::cout<<oneweibo.GetWords()<<std::endl;
		}
	}
}
//void Cluster::InsertOneTopicToDatabase(Topic &one_topic){
//
//}
