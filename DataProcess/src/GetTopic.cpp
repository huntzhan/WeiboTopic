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
#include<time.h>
#include<math.h>
#include<algorithm>
//#define DEBUG2
#define TIME
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
void GetTopic::GetEveryWordInCurrentHour() {
	int count = 0;
	list<string>::iterator m_c_iter;
	for (m_c_iter = this->m_current_messageList.begin(); m_c_iter
			!= this->m_current_messageList.end(); ++m_c_iter) {
#ifdef DEBUG1
		count++;
		std::cout<<count<<std::endl;
#endif
		Weibo oneweibo;
		string oneweiboId = *m_c_iter;

		this->dbdao->GetEveryWeiboFromDatabase(oneweiboId, oneweibo);

		vector<string> * oneWeiboContent = oneweibo.GetWords();//这里是创建了新对象呢还是直接指针
		vector<string>::iterator it;
		for (it = oneWeiboContent->begin(); it != oneWeiboContent->end(); ++it) {
			string key = *it;
			AddKeyToMap(this->m_topic_word, key, oneweiboId);
		}
	}
}
void GetTopic::GetEveryWordInCurrentHourByWordProperty() {
	this->overMapNum=0;
	int count = 0;
	list<string>::iterator m_c_iter;
	for (m_c_iter = this->m_current_messageList.begin(); m_c_iter
			!= this->m_current_messageList.end(); ++m_c_iter) {
#ifdef DEBUG1
		count++;
		std::cout<<count<<std::endl;
#endif
		Weibo oneweibo;
		string oneweiboId = *m_c_iter;

		this->dbdao->GetEveryWeiboWithProperty(oneweiboId, oneweibo);

		vector<Word> * oneWeiboContent = oneweibo.GetContentWithProperty();//这里是创建了新对象呢还是直接指针
		vector<Word>::iterator it;
		for (it = oneWeiboContent->begin(); it != oneWeiboContent->end(); ++it) {
			AddKeyToMapWithProperty(this->m_topic_word, *it, oneweiboId);
		}
	}
}
void GetTopic::AddKeyToMapWithProperty(map<string, TopicWord>&filterMap,
		Word &word, std::string weiboId) {
	std::string key=word.word;
	map<string, TopicWord>::iterator it;
	if (word.word.size()>=4&&(word.proper.compare("v")==0||word.proper.compare("vn") == 0||word.proper.compare("n") == 0||word.proper.compare("un") == 0)) {//只要名次，地点词
		it=filterMap.find(key);
		if (it == filterMap.end()) {
			TopicWord topicword(key, 1.0);
			topicword.GetWordToWeiboidList()->insert(weiboId);
			filterMap.insert(make_pair(key, topicword));
		} else {
//			this->isOverThrodandDelete(it->second);
			it->second.SetFrequency(it->second.GetFrequency() + 1);
			it->second.GetWordToWeiboidList()->insert(weiboId);
		}
	}
	if(filterMap.size()>TOPICMAPTHROD){
		this->DeleteElementsBelowThrod(filterMap);
	}
}

void GetTopic::DeleteElementsBelowThrod(map<string, TopicWord>&filterMap){
	map<string, TopicWord>::iterator map_it=filterMap.begin();

	for(;map_it!=filterMap.end();++map_it){
		if(map_it->second.m_dFrequency<10){
			filterMap.erase(map_it);
		}
	}
}
void GetTopic::CalWordIDF(){
	map<string, TopicWord>::iterator t_it = this->m_topic_word.begin();
	double IDF;
	int size=t_it->second.word_to_weiboid_list.size();
	for (; t_it != this->m_topic_word.end();++t_it){
		IDF=log((double)this->weibo_size/t_it->second.word_to_weiboid_list.size());
		double temp=t_it->second.m_dFrequency;
		t_it->second.m_dFrequency*=IDF;
		t_it->second.IDF=temp;

//		std::cout<<IDF<<std::endl;
	}
	this->m_k_messageList.clear();
}
void  GetTopic::GenTopicWordByFrequency(){
#ifdef TIME
	time_t start;
	start=time(NULL);
#endif
//	this->GetEveryWordInCurrentHour();
	this->GetEveryWordInCurrentHourByWordProperty();
#ifdef TIME
	time_t ends;
	ends=time(NULL);
	std::cout<<"获取特征词用时："<<difftime(ends,start)<<std::endl;
#endif
	this->CalWordIDF();
	this->TopicWordSort();
#ifdef DEBUG22
	printMaps(this->m_topic_word);
#endif
}
void GetTopic::GenTopicWord(){

	this->GetEveryWordInCurrentHour();//获取当前一小时内不重复的词
#ifdef TIME
	time_t start1;
	start1 = time(NULL);
#endif
	this->CalTopicWordInKhours();
#ifdef TIME
	time_t ends1;
	ends1 = time(NULL);
	std::cout << "计算特征词增长率用时：" << difftime(ends1, start1) << std::endl;
#endif
	map<string, TopicWord>::iterator t_it = this->m_topic_word.begin();

	for (; t_it != this->m_topic_word.end(); ++t_it) {
		string key = t_it->first;
		map<string, double>::iterator find_K_it = this->k_hour_topic_word.find(
				key);
		if (find_K_it != this->k_hour_topic_word.end()) {
			if (find_K_it->second == 0) {
				t_it->second.SetFrequency(
						t_it->second.GetFrequency() * this->K_WINDOW / 0.9);//如果之前都没有出现这个词，就除以比1小的数
				continue;
			}
			double tempvalue = t_it->second.GetFrequency() * this->K_WINDOW
					/ find_K_it->second;//计算词的权重
			t_it->second.SetFrequency(tempvalue);
		}
	}
	this->m_k_messageList.clear();//释放内存，因为前几个小时的基本用不上了
#ifdef DEBUG3
	printMaps(this->m_topic_word);
#endif
#ifdef TIME
	time_t start2;
	start2 = time(NULL);
#endif
	this->TopicWordSort();
#ifdef TIME
	time_t ends2;
	ends2 = time(NULL);
	std::cout << "特征词排序用时：" << difftime(ends2, start2) << std::endl;
#endif
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
#ifdef DEBUG2
	printVector(sort_vec);
#endif
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
		if(v_it->second.IDF>10){
			this->m_topic_word.insert(make_pair(key,value_topic));
		}

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


/*
 void isOverThrodandDelete(TopicWord &topicword){
	if (topicword.GetWordToWeiboidList()->size() > this->TOPICMAPTHROD) {
		this->overMapNum++;
		if (this->overMapNum >= 100) {
			this->ClearTopicWordMapOverThrod();
		}
	}
}
void GetTopic::ClearTopicWordMapOverThrod(map<string, TopicWord>&filterMap){
	map<string, TopicWord>::iterator map_it=filterMap.begin();
	for(;map_it!=filterMap.end();++map_it){
		if(map_it->second.word_to_weiboid_list.size()>this->TOPICMAPTHROD){
			std::set<std::string>::iterator set_it=map_it->second.word_to_weiboid_list.begin();
			for(;set_it!=map_it->second.word_to_weiboid_list.end();++set_it){

			}
		}
	}
}
void ReadFileAndMergeSet(TopicWord & topicword){
	std::string output;
	std::set<std::string> temp_weiboidset;
	std::string filename=topicword.m_sword;
	std::set<std::string>::iterator set_it=topicword.word_to_weiboid_list.begin();
	std::ifstream topicfile("./TopicWordFile/"+filename+".txt");
	if (!topicfile) {
//		std::cout << "open file failed" << std::endl;
		std::ofstream outputfile("./TopicWordFile/"+filename+".txt");
		for(;set_it!=topicword.word_to_weiboid_list.end();++set_it){
			outputfile << *set_it << std::endl;
		}


		outputfile.close();
	} else {
		std::string temp;
		if (!topicfile.eof()) {
			topicfile >> temp;
			std::cout << temp << std::endl;
			weiboidset.insert(temp);
		}
	}
}
 */

