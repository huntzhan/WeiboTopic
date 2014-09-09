/*************************************************************************
	> File Name: TopicSimhash.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2014年09月07日 星期日 15时32分02秒
 ************************************************************************/

#include<iostream>
#include"TopicSimhash.h"
#include"Weibo.h"
#include"algorithm"

void printVector_sort(std::vector<Weibo> sort){
	std::vector<Weibo>::iterator it = sort.begin();
	for(; it != sort.end() ; ++it){
		std::cout<<it->text<<std::endl;
		std::cout<<it->topic_simhash_distance<<std::endl;
	}
}
void printVector_unsort(std::vector<Weibo> sort){
	std::vector<Weibo>::iterator it = sort.begin();
	for(; it != sort.end() ; ++it){
		std::cout<<it->text<<endl;
	}
}
bool _sort_cmp(const Weibo &weibo1, const Weibo &weibo2){
	if(weibo1.topic_simhash_distance > weibo2.topic_simhash_distance)
		return true;
	return false;
}
unsigned int  TopicSimhash::CalTopicWordSimhash(Topic &one_topic, SimHash &simhash){
	std::vector<std::string> topicwords ;
	std::list<TopicWord>::iterator it = one_topic.m_stopic.begin();
	
	for(;it != one_topic.m_stopic.end();++it){
		topicwords.push_back(it->m_sword);
	}

	unsigned int  topicwordsimhash =simhash.BlogHashAfterParser(topicwords);	
	return topicwordsimhash;
}
void TopicSimhash::CalSimHashAndDeleteWeibo(Topic &one_topic){
	SimHash simhash;
	//先计算话题特征词的simhash值
	unsigned int topicwordsimhash =this-> CalTopicWordSimhash(one_topic,simhash);
	
	//用于将微博根据simhash排序
	std::vector<Weibo> sort_weibo;
	std::list<Weibo>::iterator weiboit=	one_topic .topic_weibo.begin();
	for(;weiboit != one_topic.topic_weibo.end();++weiboit){
        unsigned int s_value=simhash.BlogHash(weiboit->spilt.c_str());
		weiboit->topic_simhash_distance=simhash.Calculate_Distance(s_value,topicwordsimhash);
		Weibo weibo = *weiboit;
	    sort_weibo.push_back(weibo);	
	}
	printVector_unsort(sort_weibo);
	std::sort(sort_weibo.begin(),sort_weibo.end(),_sort_cmp);
	printVector_sort(sort_weibo);
	one_topic.topic_weibo.clear();
	std::vector<Weibo>::iterator sort_it = sort_weibo.begin();
	for(;sort_it != sort_weibo.end(); ++sort_it){
		one_topic.topic_weibo.push_back(*sort_it);
	}

}
