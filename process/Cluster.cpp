/*
 * cluster.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: hogachen
 */


#include"Cluster.h"
#include "CooccurrenceWord.h"
#include"TopicWord.h"
#include"DBdao.h"
#include<iostream>
#include<string>
using namespace std;
#define THROD 2.0
#define MINVALUE -1
#define DEBUG_CLUSTER
typedef map<string,double> MAP;

void Cluster::CalWordsCooccurrence(){
	
	list<string>::iterator weiboit= this->dbdao.weibo_id_list.begin();
	for(;weiboit!= this->dbdao.weibo_id_list.end();++weiboit){
		string weiboID = *weiboit;
		Weibo oneweibo;
		this->dbdao.GetEveryWeiboFromDatabase(weiboID,oneweibo);
		std::vector<std::string>*vec = oneweibo.GetWords();
		std::vector<std::string>::iterator vec_it = vec->begin();
		std::vector<std::string>word_coou_oneweibo;
		for(;vec_it!=vec->end();++vec_it){
			std::string oneword = * vec_it;
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
					find_it->second=1.0;
				}
			}
		}
	}
}

void Cluster::Singlepass(){
	this->CalWordsCooccurrence();
#ifdef DEBUG_CLUSTER
	std::cout<<"CalWordsCooccurrence finished!"<<std::endl;
#endif
//	vector<Topic> v_clusterList;

	MAP::iterator topic_w_it = this->topicword->begin();
	TopicWord t_word(topic_w_it->first,topic_w_it->second);

	Topic topic(t_word);
	topic_w_it++;
	for(;topic_w_it != this->topicword->end();++topic_w_it){

		TopicWord t_words(topic_w_it->first,topic_w_it->second);
#ifdef DEBUG_CLUSTER
	std::cout<<"singlepass process!"<<std::endl;
#endif

		vector<Topic>::iterator vec_clu_it=this->clusterList.begin();
		double maxDistance=MINVALUE;
		vector<Topic>::iterator belong_clus_it=vec_clu_it;//这里应该是赋值还是指针？？？？
		for(;vec_clu_it != this->clusterList.end();++vec_clu_it){
//			Topic temptopic=;
			double words_distance=Cal_Words_Topic_Distance(*vec_clu_it,t_words);
			if(maxDistance < words_distance){
				maxDistance = words_distance;
				belong_clus_it=vec_clu_it;
			}
		}
		if(maxDistance < THROD){
			Topic newTopic(t_words);
			this->clusterList.push_back(newTopic);//这里虽然newTopic是局部变量，但是由于会复制一个新的，有类的时候会动态调用拷贝构造函数
		}else{
			belong_clus_it->addTopicWord(t_words);
		}
	}
}
double Cluster::Cal_Words_Topic_Distance(Topic &topic,TopicWord t_word){
#ifdef DEBUG_CLUSTER
	std::cout<<"cal distance process!"<<std::endl;
#endif
	double topic_word_dis=0.0;
	std::string keyword = t_word.GetTopicWord();
	std::map<std::string,double>* map_map;
	std::map<string,CooccurrenceWord>::iterator co_it=co_ccur_matrix.find(keyword);
	if(co_it!=co_ccur_matrix.end()){
//		CooccurrenceWord tempCooc=co_it->second;
		map_map=co_it->second.GetWordCooccurrence();
	}
	else {
		return 0.0;
	}
	vector<TopicWord>::iterator clu_it=topic.GetsTopic().begin();
	std::string one_topic_word;
	std::map<std::string,double>::iterator second_it;
	for(;clu_it!=topic.GetsTopic().end();++clu_it){
		one_topic_word= clu_it->GetTopicWord();
		second_it=map_map->find(one_topic_word);
		if(second_it!=map_map->end()){
			topic_word_dis+=second_it->second;
		}
	}
	std::cout<<topic_word_dis<<std::endl;
	return topic_word_dis;
}
