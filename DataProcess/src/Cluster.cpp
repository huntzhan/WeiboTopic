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
#include<unistd.h>
#include<time.h>
#include<string>
using namespace std;
//#define THROD 3.0
#define MINVALUE -1
#define TIME
//#define BELONT_TOPIC_THROD 2.0
//#define DEBUG_CLUSTER1
//#define CALDIS
//#define PRINTTOPIC2

typedef map<string,TopicWord> MAP;
bool TopicCmp(const Topic &topic1,const Topic &topic2){
	if(topic1.topic_message_num>topic2.topic_message_num)return true;
	return false;
}
bool weibosort(const subword & s1,const subword & s2){
	if(s1.fre>s2.fre)return true;
	return false;
}
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
#ifdef TIME
	time_t start3;
	start3=time(NULL);
#endif
	this->CalWordsCooccurrence();
#ifdef TIME
	time_t ends3;
	ends3=time(NULL);
	std::cout<<"计算特征词共现度用时："<<difftime(ends3,start3)<<std::endl;
#endif
	this->SetClusterThrod(this->GenClusterThrod()+THROD_ADD);
	std::cout<<"CLUSTER_THROD: "<<this->CLSTER_THROD<<std::endl;
#ifdef DEBUG_CLUSTER1
	printMatrix(this->co_ccur_matrix);
#endif

//	vector<Topic> v_clusterList;

	MAP::iterator topic_w_it = this->topicword->begin();
	TopicWord firstword=topic_w_it->second;
	Topic topic;
	topic.TopicInit(firstword);
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
		if(maxDistance < this->CLSTER_THROD){
			Topic newTopic;
			newTopic.TopicInit(topic_w_it->second);
			this->clusterList.push_back(newTopic);//这里虽然newTopic是局部变量，但是由于会复制一个新的，有类的时候会动态调用拷贝构造函数
		}else{
			belong_clus_it->addTopicWord(topic_w_it->second);
		}
	}
#ifdef TIME
	time_t ends4;
	ends4=time(NULL);
	std::cout<<"一趟聚类用时："<<difftime(ends4,start3)<<std::endl;
#endif

#ifdef TIME
	time_t start5;
	start5=time(NULL);

#endif
	this->ListAllTopicWeiboId();
//	sleep(3);
#ifdef TIME
	time_t ends5;
	ends5=time(NULL);
	std::cout<<"微博对应话题用时："<<difftime(ends5,start5)<<std::endl;
#endif
	this->SortTopic();
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
	return topic_word_dis/topic.GetsTopic()->size();
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
	one_topic.topic_message_num=0;
	for(;topic_it!= one_topic.GetsTopic()->end();++topic_it){
		std::set<std::string>::iterator topicword_weibolist_it = topic_it->GetWordToWeiboidList()->begin();
		for(;topicword_weibolist_it!=topic_it->GetWordToWeiboidList()->end();++topicword_weibolist_it){
			std::string weiboid=*topicword_weibolist_it;
			topic_weibo_id_map_it=topic_weibo_id_map->find(weiboid);
			if(topic_weibo_id_map_it!=topic_weibo_id_map->end()){
				topic_weibo_id_map_it->second=topic_weibo_id_map_it->second+1;
//				if(this->BELONG_TOPIC_THROD>=2){
//					one_topic.topic_message_num+=1;//按消息量进行排序     //这是是聚类好的话题簇中，至少有两个词同时出现在这条微博，才将该微博加入该话题
//					one_topic.GetWeiboIdList()->push_back(weiboid);
//				}
			}else{
				topic_weibo_id_map->insert(make_pair(weiboid,1.0));
//				if(this->BELONG_TOPIC_THROD<2){
//					one_topic.topic_message_num+=1;//按消息量进行排序在这里是出现一次就算      //这是是聚类好的话题簇中，只要一个词出现在这条微博，才将该微博加入该话题
//					one_topic.GetWeiboIdList()->push_back(weiboid);
//				}
			}
		}
	}
	topic_weibo_id_map_it=topic_weibo_id_map->begin();

	for(;topic_weibo_id_map_it!=topic_weibo_id_map->end();++topic_weibo_id_map_it){
		if(topic_weibo_id_map_it->second>=this->BELONG_TOPIC_THROD){
			subword sw(topic_weibo_id_map_it->first,topic_weibo_id_map_it->second);
			one_topic.GetWeiboIdList()->push_back(sw);
			one_topic.topic_message_num+=1;
		}
	}
	std::sort(one_topic.GetWeiboIdList()->begin(),one_topic.GetWeiboIdList()->end(),weibosort);

}

void Cluster::InsertTopicToDatabase(Topic &one_topic){
	std::map<std::string,double>*topic_weibo_id_map=one_topic.GetTopicWeiboId();
	double weiboid_num=0.0;
	std::string weiboid;
	std::map<std::string,double>::iterator topic_weibo_id_map_it;
	for(;topic_weibo_id_map_it!=topic_weibo_id_map->end();++topic_weibo_id_map_it){
		weiboid=topic_weibo_id_map_it->first;
		weiboid_num=topic_weibo_id_map_it->second;
		if(weiboid_num>this->BELONG_TOPIC_THROD){
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
void Cluster::SortTopic(){
	std::sort(this->clusterList.begin(),this->clusterList.end(),TopicCmp);
}
std::vector<int> Cluster::GenRandomValue(){
	int MAX=this->GetCooccurrence()->size()-1;
	if(MAX>RAND_SIZE)randsize=RAND_SIZE;
	else randsize=MAX;
	srand((unsigned) time(NULL));
	std::vector<int> v_i;
	for(int i=0;i<randsize;i++){
//		std::cout<<rand()%MAX<<std::endl;
		v_i.push_back(rand()%MAX);
	}
	return v_i;
}
double Cluster:: GenClusterThrod(){
	double tempthrod=0.0;
	std::vector<std::string> tempvec;
	std::map<std::string,CooccurrenceWord>::iterator it = this->GetCooccurrence()->begin();
	for(;it!=this->GetCooccurrence()->end();++it){
		tempvec.push_back(it->first);
	}

	std::vector<int > tempintvec=this->GenRandomValue();
	for(int i=0;i<tempintvec.size();++i){
		int index = tempintvec[i];
		it=this->GetCooccurrence()->find(tempvec[index]);
		tempthrod +=it->second.GetWordCooccurrence()->begin()->second;
	}
	return tempthrod/randsize;
}
