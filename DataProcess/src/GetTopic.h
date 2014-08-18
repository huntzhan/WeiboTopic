/*
 * GetTopic.h
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
 */

#ifndef GETTOPIC_H_
#define GETTOPIC_H_

/*
 * 可以改进的地方：可以不必每次都计算七个小时的频率，只要减去最前一个小时就可以了
 */
#include"DBoperation.h"
#include "Weibo.h"
#include "TopicWord.h"
//#include"DBdao.h"
#include"OneWeibo.h"
#include<list>
#include<set>
#include<map>
#include<iostream>
using namespace std;

typedef pair<string,TopicWord> PAIR;

bool SortCmp (const PAIR & key1,const PAIR & key2);

class GetTopic{
	int TOPIC_WORD_NUM;									       	//想要得到前几个主题词，这个默认是前1000个
	int K_WINDOW;										 		//时间窗口的大小

	map<string,TopicWord> m_topic_word;							//提取出的没有重复词列表，包括单词和词的权值

	DBoperation *dboper;
	int TOPICMAPTHROD;

public:

	int overMapNum;



	map<string,TopicWord> * GetTopicWord(){
		return &m_topic_word;
	}
	void SetDBdao(DBoperation *dboper){
		this->dboper=dboper;
	}
	void GetEveryWordInCurrentHour();

    void TopicWordSort();								        //根据获得的主题词列表与之前的K个时间窗口内的词的权重比较之后按照权重排序，最后更新主题词指针

    void AddKeyToMap(map<string,TopicWord>&filterMap,string key,std::string weiboId);

    void CalTopicWordInKhours();



    void GenTopicWord();
    void GenTopicWordByFrequency();
    void InitGetTopic(DBoperation *dboper,int TOPIC_WORD_NUM,int K_WINDOW,int TOPICMAPTHROD){
		this->TOPIC_WORD_NUM = TOPIC_WORD_NUM;
		this->K_WINDOW = K_WINDOW;
		this->TOPICMAPTHROD=TOPICMAPTHROD;
		SetDBdao(dboper);

    }

    void GetEveryWordInCurrentHourByWordProperty();
    void AddKeyToMapWithProperty(map<string, TopicWord>&filterMap,
    		Word &key);

    void CalWordIDF();
    void DeleteElementsBelowThrod(map<string, TopicWord>&filterMap);
    bool ChangeToNextTable(int &firstcount,int & count);
    void ChangeOneTableCount(int &firstcount,int & count);
};

#endif /* GETTOPIC_H_ */
