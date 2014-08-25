/*
 * TopicViewAndPolitics.h
 *
 *  Created on: 2014年8月24日
 *      Author: Administrator
 */

#ifndef TOPICVIEWANDPOLITICS_H_
#define TOPICVIEWANDPOLITICS_H_
#include"Topic.h"
#include"TopicView.h"
#include"IsPolitics.h"
class TopicViewAndPolitics{
  //生成子话题
  TopicView tw;
  //判断是否为政治话题
  IsPolitics ispo;

  DBoperation * dboper;
  std::vector<Topic>*clusterList;
  int NUM_OF_SUB_WORD;
  int MIN_TOPIC_MESSAGE_NUM;
  int TOPICVIEW_WEIBO_NUM;

public:
  void InitTopicViewAndPolitics(DBoperation * dboper,std::vector<Topic>*clusterList,int NUM_OF_SUB_WORD,
      int MIN_TOPIC_MESSAGE_NUM,int TOPICVIEW_WEIBO_NUM){
    this->dboper=dboper;
    this->clusterList=clusterList;
    this->NUM_OF_SUB_WORD=NUM_OF_SUB_WORD;
    this->MIN_TOPIC_MESSAGE_NUM=MIN_TOPIC_MESSAGE_NUM;
    this->TOPICVIEW_WEIBO_NUM=TOPICVIEW_WEIBO_NUM;
  }
  void InsterAllTopicToDatabase();
  void InsertTopicToDatabase(Topic &one_topic);
};




#endif /* TOPICVIEWANDPOLITICS_H_ */
