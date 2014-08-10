/*
 * GetTopic.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
 */

#include "GetTopic.h"
#include "DEBUG.h"
#include "Weibo.h"
#include "DBdao.h"
#include<algorithm>
//#define DEBUG5
using namespace std;


bool SortCmp (const PAIR & key1,const PAIR & key2){
	if(key1.second.m_dFrequency > key2.second.m_dFrequency)return true;
	return false;
//	if(key1.second <key2.second)return -1;
//	return 0;
}


void GetTopic::AddKeyToMap(map<string,TopicWord>&filterMap,string key,std::string weiboId){
	map<string,TopicWord>::iterator it = filterMap.find(key);
	if(it==filterMap.end()){
		TopicWord topicword(key,1.0);
		topicword.GetWordToWeiboidList()->insert(weiboId);
//		std::cout<<"insert one weiboid"<<std::endl;
		filterMap.insert(make_pair(key,topicword));
	}
	else {
		it->second.SetFrequency(it->second.GetFrequency()+1);
		it->second.GetWordToWeiboidList()->insert(weiboId);
//		std::cout<<"insert sexond weiboid"<<std::endl;
	}
}
/*
 * return 是销毁并创建一个新的对象返回
 */
void GetTopic::GetEveryWordInCurrentHour(){
//	this->GetCurrentHourWeiboList("time");//获取当前时间段微博ID列表
	std::set<std::string> stop_word_set;
	MakeStopSet(stop_word_set);


	int count=0;
	list<string>::iterator m_c_iter;
	for(m_c_iter=this->m_current_messageList.begin();
			m_c_iter!=this->m_current_messageList.end();++m_c_iter){
#ifdef DEBUG1
		count++;
		std::cout<<count<<std::endl;
#endif
		Weibo oneweibo;
		string oneweiboId=*m_c_iter;



		this->dbdao->GetEveryWeiboFromDatabase(oneweiboId,oneweibo);



		vector<string> * oneWeiboContent=oneweibo.GetWords();//这里是创建了新对象呢还是直接指针
		vector<string>::iterator it;
		for(it=oneWeiboContent->begin();it!=oneWeiboContent->end();++it){
			string key=*it;
			AddKeyToMap(this->m_topic_word,key,oneweiboId);
		}
	}
#ifdef DEBUG2
printMaps(this->m_topic_word);
#endif
}

void GetTopic::GenTopicWord(){

	this->GetEveryWordInCurrentHour();//获取当前一小时内不重复的词
	this->CalTopicWordInKhours();
	map<string,TopicWord>::iterator t_it=this->m_topic_word.begin();

	for(;t_it!=this->m_topic_word.end();++t_it){
		string key=t_it->first;
		map<string,double>::iterator find_K_it=this->k_hour_topic_word.find(key);
		if(find_K_it!=this->k_hour_topic_word.end()){
			if(find_K_it->second==0){
				t_it->second.SetFrequency(t_it->second.GetFrequency()*this->K_WINDOW/0.9);//如果之前都没有出现这个词，就除以比1小的数
				continue;
			}
			double tempvalue=t_it->second.GetFrequency()*this->K_WINDOW/find_K_it->second;//计算词的权重
			t_it->second.SetFrequency(tempvalue);
		}
	}
#ifdef DEBUG3
printMaps(this->m_topic_word);
#endif
	this->TopicWordSort();
}


/*
 * 直接根据成员函数进行排序
 */
void GetTopic::TopicWordSort(){
	map<string,TopicWord>::iterator mapIt;
	vector<PAIR> sort_vec;
	for(mapIt=this->m_topic_word.begin();mapIt!=this->m_topic_word.end();++mapIt){
		sort_vec.push_back(make_pair(mapIt->first,mapIt->second));
	}
#ifdef DEBUG5
std::cout<<"sort_vec size is : "<<sort_vec.size()<<std::endl;
#endif
	sort(sort_vec.begin(),sort_vec.end(),SortCmp);

//	map<string,double> map_topicwordResult;

	int topicWordNum= this->m_topic_word.size();
	this->m_topic_word.clear();
	if(topicWordNum > TOPIC_WORD_NUM)
		topicWordNum=TOPIC_WORD_NUM;
	vector<PAIR>::iterator v_it=sort_vec.begin();
	int count=0;
	while(v_it!=sort_vec.end()&&count<topicWordNum){
		string key=v_it->first;
		TopicWord value_topic=v_it->second;
#ifdef DEBUG5
std::cout<<"after sort : "<<key<<"		weibolist size: "<<*value_topic.word_to_weiboid_list.begin()<<std::endl;
#endif
		this->m_topic_word.insert(make_pair(key,value_topic));
		++v_it;
		++count;
	}
#ifdef DEBUG5
std::cout<<"normal  2"<<std::endl;
printMaps(this->m_topic_word);
#endif
	//这里将微博主题词列表指向新的对象，因为是临时变量，所以只要退出了空间，变量就会被销毁
//	this->m_topic_word=map_topicwordResult
}
/*
 * 计算取出来的词在当前小时内的频率
 */
void GetTopic::CalTopicWordInKhours(){

	std::set<std::string> stop_word_set;
	 MakeStopSet(stop_word_set);
//	this->dbdao.GetKHourWeiboList("from" ,6,this->m_k_messageList);
	map<string,TopicWord >::iterator  t_w_it=this->m_topic_word.begin();
	for(;t_w_it!=this->m_topic_word.end();++t_w_it){
		this->k_hour_topic_word.insert(make_pair(t_w_it->first,0.0));
	}
int count=0;
	list<string>::iterator m_K_it = this->m_k_messageList.begin();
	for(;m_K_it!=this->m_k_messageList.end();++m_K_it){
//#ifdef DEBUG
//		count++;
//		std::cout<<count<<std::endl;
//#endif
		string weiboID = *m_K_it;
		Weibo oneWeibo;


		this->dbdao->GetEveryWeiboFromDatabase(weiboID,oneWeibo);



		vector<string>* oneWeiboContent=oneWeibo.GetWords();//这里是直接引用原有的微博内容的对象
		vector<string>::iterator it;
		for(it=oneWeiboContent->begin();it!=oneWeiboContent->end();++it){
			string key=*it;
			map<string ,double>::iterator findIt=this->k_hour_topic_word.find(key);
			if(findIt!=this->k_hour_topic_word.end()){
				findIt->second=findIt->second+1.0;
			}
		}
	}
//#ifdef DEBUG
//		printMaps(this->k_hour_topic_word);
//#endif
	/**
	 * 这样释放内存？？？？？？还是将内存泄漏了？？？
	 */
//	map<string,double> temp_map ;
//	this->k_hour_topic_word=temp_map;

}
