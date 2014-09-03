/*
 *  GetTopic.h
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
 *  description：
 *  	采用词频和词性获取特征词，同时两两计算共现度，最后利用singlepass（一趟聚类）进行聚类
 */

#ifndef GETTOPIC_H_
#define GETTOPIC_H_

#include "DBoperation.h"
#include "Weibo.h"
#include "TopicWord.h"
#include "OneWeibo.h"
#include<list>
#include<set>
#include<map>
#include<iostream>

typedef pair<std::string, TopicWord> PAIR;

bool SortCmp(const PAIR &key1, const PAIR &key2);

class GetTopic {
  //想要得到前几个主题词，这个默认是前1000个
  int TOPIC_WORD_NUM;

  //提取出的没有重复词列表，包括单词和词的权值
  map<string, TopicWord> m_topic_word;

  //数据库统一操作类
  DBoperation *dboper;

  //提起特征词的时候，会出现大量的词，为确保速度和防止内存占用太多，将超过某个阈值的词删除掉
  int TOPICMAPTHROD;

public:

  int overMapNum;

  map<string, TopicWord> *GetTopicWord() {
    return &m_topic_word;
  }

  /*@description：
   *  初始化提取特征词的各个参数
   *@input:
   *  传入的数据库指针对象：dboper
   *  待聚类的特征词个数：TOPIC_WORD_NUM
   *  当存放特征词map个数超过该阈值时删除部分没有用的特征词:TOPICMAPTHROD
   */
  void InitGetTopic(DBoperation *dboper, int TOPIC_WORD_NUM, int TOPICMAPTHROD) {
    this->TOPIC_WORD_NUM = TOPIC_WORD_NUM;
    this->TOPICMAPTHROD = TOPICMAPTHROD;
    SetDBdao(dboper);
  }

  void SetDBdao(DBoperation *dboper) {
     this->dboper = dboper;
   }

  //根据词性获取特征词的主函数，其他函数被它调用
  void GenTopicWordByFrequency();

  //根据词性提取特征词
  void GetEveryWordInCurrentHourByWordProperty();

  //根据获得的主题词列表与之前的K个时间窗口内的词的权重比较之后按照权重排序，最后更新主题词指针
  void TopicWordSort();


  void AddKeyToMapWithProperty(map<std::string, TopicWord> &filterMap, Word &key);

  void DeleteElementsBelowThrod(map<std::string, TopicWord> &filterMap);

};

#endif /* GETTOPIC_H_ */
