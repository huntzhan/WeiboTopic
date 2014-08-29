/*
 * TopicViewAndPolitics.h
 *
 *  Created on: 2014年8月24日
 *      Author: Administrator
 *  description：
 *      根据话题下前TOPICVIEW_WEIBO_NUM条（50）微博，进行话题概要提取，话题是否为政治类判别
 */

#ifndef TOPICVIEWANDPOLITICS_H_
#define TOPICVIEWANDPOLITICS_H_
#include"Topic.h"
#include"TopicView.h"
#include"IsPolitics.h"
class TopicViewAndPolitics{
  //生成子话题对象
  TopicView tw;

  //判断是否为政治话题对象
  IsPolitics ispo;

  DBoperation * dboper;
  std::vector<Topic>*clusterList;

  //提取子话题时提取的单位词的个数
  int NUM_OF_SUB_WORD;

  //话题下的微博数小于这个数时该话题不存入数据库
  int MIN_TOPIC_MESSAGE_NUM;

  //话题下观点提取代表性的微博数
  int TOPICVIEW_WEIBO_NUM;

public:
  //初始化函数，设置参数
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
  void GetOneTopicWeiboByBatch(Topic &one_topic, int topicnum);
  void printMessage(Topic &one_topic, int topicnum);
  void QueryWeiboIntime( std::map<string ,std::list<std::string> >&table_to_weibo,
      std::list<Weibo> &weibocontentlist);
};




#endif /* TOPICVIEWANDPOLITICS_H_ */
