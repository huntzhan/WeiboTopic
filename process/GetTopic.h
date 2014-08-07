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

#include "Weibo.h"
#include "TopicWord.h"
#include"DBdao.h"
#include<list>
#include<set>
#include<map>
#include<iostream>
using namespace std;

typedef pair<string,double> PAIR;

bool SortCmp (const PAIR & key1,const PAIR & key2);

class GetTopic{
	int TOPIC_WORD_NUM;									       	//想要得到前几个主题词，这个默认是前1000个
	int K_WINDOW;										 		//时间窗口的大小
	list<string> m_current_messageList;							//根据时间获取到的微博ID列表
	list<string> m_k_messageList;
	map<string,double> m_topic_word;							//提取出的没有重复词列表，包括单词和词的权值
	map<string,double> k_hour_topic_word;						//前K个小时的单词的出现频率，词跟m_topicword内容一样,用完之后要释放掉
	DBdao dbdao;
public:
	map<string,double> * GetTopicWord(){
		return &m_topic_word;
	}
	void SetDBdao(DBdao &dbdao){
		this->dbdao=dbdao;
		this->m_current_messageList=this->dbdao.weibo_id_list;
		this->m_k_messageList=this->dbdao.k_hours_weibo_id_list;
	}
	void GetEveryWordInCurrentHour();

    void TopicWordSort();								        //根据获得的主题词列表与之前的K个时间窗口内的词的权重比较之后按照权重排序，最后更新主题词指针

    void AddKeyToMap(map<string,double>&filterMap,string key);

    void CalTopicWordInKhours();



    void GenTopicWord();
    GetTopic(DBdao & dbdao,int TOPIC_WORD_NUM,int K_WINDOW){
    	this->TOPIC_WORD_NUM=TOPIC_WORD_NUM;
    	this->K_WINDOW=K_WINDOW;
    	SetDBdao(dbdao);
    }
};

#endif /* GETTOPIC_H_ */
