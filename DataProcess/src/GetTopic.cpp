/*
 * GetTopic.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
 */

#include "GetTopic.h"
#include "DEBUG.h"
#include "Weibo.h"
#include<time.h>
#include<math.h>
#include<algorithm>
//#define COUNT
//#define DEBUG
#define TIME
using namespace std;

bool SortCmp(const PAIR & key1, const PAIR & key2) {
	if (key1.second.m_dFrequency > key2.second.m_dFrequency)
		return true;
	return false;
}

void GetTopic::GetEveryWordInCurrentHourByWordProperty() {
	std::list<OneWeibo> resultword;
	std::list<OneWeibo>::iterator or_it;
	int count=0;
	int num=0;
	while(true){
		if(!this->dboper->ChangeToNextTable(count))break;
#ifdef COUNT
		if (count > 10000)
			break;
#endif
		this->dboper->ChangeOneTableCount(count);

		resultword.clear();
		this->dboper->GetMidandText(count,this->dboper->OneTimeReadWeiboNum,resultword);;
		or_it=resultword.begin();
		for(;or_it!=resultword.end();++or_it){
			num++;
			std::list<Word>::iterator it= or_it->words.begin();
			for(;it!=or_it->words.end();++it){
				this->AddKeyToMapWithProperty(this->m_topic_word, *it);
			}
		}
	}
	std::cout<<"处理数据："<<num<<"条"<<std::endl;
}
void GetTopic::AddKeyToMapWithProperty(map<string, TopicWord>&filterMap,
		Word &word) {
	std::string key = word.word;
	map<string, TopicWord>::iterator it;
	if (word.word.size() >= 4 && (word.proper.compare("v") == 0
			|| word.proper.compare("vn") == 0 || word.proper.compare("n") == 0
			|| word.proper.compare("un") == 0)) {//只要名词，地点词
		it = filterMap.find(key);
		if (it == filterMap.end()) {
			TopicWord topicword(key, 1.0);
			filterMap.insert(make_pair(key, topicword));
			//这里有个很重要的结论：23000之后，词的个数基本不增长
//			std::cout<<"map size:"<<this->m_topic_word.size()<<std::endl;
		} else {
			it->second.SetFrequency(it->second.GetFrequency() + 1);
//			std::cout<<"insert2222"<<std::endl;
		}
	}
	if (filterMap.size() > TOPICMAPTHROD) {
		this->DeleteElementsBelowThrod(filterMap);
		std::cout<<"delete the key that below 10...."<<std::endl;
	}
}

void GetTopic::DeleteElementsBelowThrod(map<string, TopicWord>&filterMap) {
	map<string, TopicWord>::iterator map_it = filterMap.begin();

	for (; map_it != filterMap.end(); ++map_it) {
		if (map_it->second.m_dFrequency < 10) {
			filterMap.erase(map_it);
		}
	}
}

void GetTopic::GenTopicWordByFrequency() {
#ifdef TIME
	time_t start;
	start = time(NULL);
#endif
	this->GetEveryWordInCurrentHourByWordProperty();
#ifdef TIME
	time_t ends;
	ends = time(NULL);
	std::cout << "获取特征词用时：" << difftime(ends, start) << std::endl;
#endif
//	this->CalWordIDF();
//#ifdef DEBUG2
//	printMaps(this->m_topic_word);
//#endif
	this->TopicWordSort();
}


/*
 * 直接根据成员函数进行排序
 */
void GetTopic::TopicWordSort() {

	map<string, TopicWord>::iterator mapIt;
	vector<PAIR> sort_vec;
	for (mapIt = this->m_topic_word.begin(); mapIt != this->m_topic_word.end(); ++mapIt) {
		sort_vec.push_back(make_pair(mapIt->first, mapIt->second));
//		std::cout<<"in sort"<<std::endl;
	}

	sort(sort_vec.begin(), sort_vec.end(), SortCmp);
#ifdef DEBUG2
	printVector(sort_vec);
#endif

	int topicWordNum = this->m_topic_word.size();
	this->m_topic_word.clear();
	if (topicWordNum > TOPIC_WORD_NUM)
		topicWordNum = TOPIC_WORD_NUM;
	vector<PAIR>::iterator v_it = sort_vec.begin();
	int count = 0;
	while (v_it != sort_vec.end() && count < topicWordNum) {
		string key = v_it->first;
		TopicWord value_topic = v_it->second;
		if (v_it->second.m_dFrequency> 10) {
			this->m_topic_word.insert(make_pair(key, value_topic));
		}
		++v_it;
		++count;
	}
}


