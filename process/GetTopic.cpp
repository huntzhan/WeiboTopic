/*
 * GetTopic.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
 */

#include "GetTopic.h"
#include "Weibo.h"
#include<algorithm>
using namespace std;

void GetTopic::GetCurrentHourWeiboList(string time){

}
void GetTopic::GetKHourWeiboList(string from ,string to){

}

int GetTopic::SortCmp (const PAIR & key1,const PAIR & key2){
	if(key1.second >key2.second)return 1;
	if(key1.second <key2.second)return -1;
	return 0;
}


void GetTopic::AddKeyToMap(map<string,double>&filterMap,string key){
				map<string,double>::iterator it = filterMap.find(key);
				if(it==filterMap.end()){
					filterMap.insert(make_pair(key,1.0));
				}

				else {
					it->second=it->second+1;
				}
}
/*
 * return 是销毁并创建一个新的对象返回
 */
void GetTopic::GetEveryWordInCurrentHour(){
	this->GetCurrentHourWeiboList("time");//获取当前时间段微博ID列表
	list<string>::iterator m_c_iter;
	for(m_c_iter=this->m_current_messageList.begin();
			m_c_iter!=this->m_current_messageList.end();++m_c_iter){

		string oneWeiboId=*m_c_iter;
		Weibo oneWeibo = GetEveryWeiboFromDatabase(oneWeiboId);
		vector<string> * oneWeiboContent=oneWeibo.GetWords();//这里是创建了新对象呢还是直接指针
		vector<string>::iterator it;
		for(it=oneWeiboContent->begin();it!=oneWeiboContent->end();++it){
			string key=*it;
			AddKeyToMap(this->m_topic_word,key);
		}
	}
}

void GetTopic::GenTopicWord(){

	this->GetEveryWordInCurrentHour();//获取当前一小时内不重复的词
	this->CalTopicWordInKhours();
	map<string,double>::iterator t_it=this->m_topic_word.begin();

	for(;t_it!=this->m_topic_word.end();++t_it){
		string key=t_it->first;
		map<string,double>::iterator find_K_it=this->k_hour_topic_word.find(key);
		if(find_K_it!=this->k_hour_topic_word.end()){
			double tempvalue=t_it->second*this->K_WINDOW/find_K_it->second;
			t_it->second=tempvalue;
		}
	}
	this->TopicWordSort();
}

/**
 * 调用进发的数据库DAO接口
 */
Weibo GetTopic::GetEveryWeiboFromDatabase(string weiboID){
	Weibo weibo;
	return weibo;
}

/*
 * 直接根据成员函数进行排序
 */
void GetTopic::TopicWordSort(){
	map<string,double>::iterator mapIt;
	vector<PAIR> sort_vec;
	for(mapIt=this->m_topic_word.begin();mapIt!=this->m_topic_word.end();++mapIt){
		sort_vec.push_back(make_pair(mapIt->first,mapIt->second));
	}
	sort(sort_vec.begin(),sort_vec.end());

	map<string,double> map_topicwordResult;
	int topicWordNum= this->m_topic_word.size();
	if(topicWordNum > TOPIC_WORD_NUM)
		topicWordNum=TOPIC_WORD_NUM;
	vector<PAIR>::iterator v_it=sort_vec.begin();
	int count=0;
	while(v_it!=sort_vec.end()&&count<topicWordNum){
		string key=v_it->first;
		double value=v_it->second;
		map_topicwordResult.insert(make_pair(key,value));
	}
	//这里将微博主题词列表指向新的对象，原来对象是否释放？？
	this->m_topic_word=map_topicwordResult;
}
/*
 * 计算取出来的词在千米昂的概率
 */
void GetTopic::CalTopicWordInKhours(){//这里不能用引用传值


	this->GetKHourWeiboList("time_from","time_to");//???????????????????????????????这里需要实现函数去获取前
	map<string,double >::iterator  t_w_it=this->m_topic_word.begin();
	for(;t_w_it!=this->m_topic_word.end();++t_w_it){
		this->k_hour_topic_word.insert(make_pair(t_w_it->first,0.0));
	}

	list<string>::iterator m_K_it = this->m_k_messageList.begin();
	for(;m_K_it!=this->m_k_messageList.end();++m_K_it){
		string weiboID = *m_K_it;
		Weibo oneWeibo = GetEveryWeiboFromDatabase(weiboID);
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

	/**
	 * 这样释放内存？？？？？？还是将内存泄漏了？？？
	 */
	map<string,double> temp_map ;
	this->k_hour_topic_word=temp_map;

}
