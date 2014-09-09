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
#include"TopicSimhash.h"
#include"Topic.h"
#include"TopicView.h"
#include"IsPolitics.h"
#include"CooccurrenceWord.h"
#include<fstream>
class TopicViewAndPolitics{
  //生成子话题对象
  TopicView tw;

  //判断是否为政治话题对象
  IsPolitics ispo;


  //用simhash计算话题下的微博列表，以此delete掉一些杂乱的微博
  TopicSimhash topicsimhash;


  DBoperation * dboper;
  std::vector<Topic>*clusterList;

  //保存的共现度矩阵
  std::map<std::string,CooccurrenceWord> *co_ccur_matrix;
  //提取子话题时提取的单位词的个数
  int NUM_OF_SUB_WORD;

  //话题下的微博数小于这个数时该话题不存入数据库
  int MIN_TOPIC_MESSAGE_NUM;

  //话题下观点提取代表性的微博数
  int TOPICVIEW_WEIBO_NUM;

  //输出文件的名字
  std::string output_filename;

public:
  //初始化函数，设置参数
  void InitTopicViewAndPolitics(DBoperation * dboper,std::vector<Topic>*clusterList,int NUM_OF_SUB_WORD,
      int MIN_TOPIC_MESSAGE_NUM,int TOPICVIEW_WEIBO_NUM,std::map<std::string,CooccurrenceWord> *co_ccur_matrix,
      std::string output_filename){
    this->dboper=dboper;
    this->clusterList=clusterList;
    this->NUM_OF_SUB_WORD=NUM_OF_SUB_WORD;
    this->MIN_TOPIC_MESSAGE_NUM=MIN_TOPIC_MESSAGE_NUM;
    this->TOPICVIEW_WEIBO_NUM=TOPICVIEW_WEIBO_NUM;
    this->co_ccur_matrix=co_ccur_matrix;
    this->output_filename=output_filename;
  }

  void InsterAllTopicToDatabase();
  void InsertTopicToDatabase(Topic &one_topic);
  void GetOneTopicWeiboByBatch(Topic &one_topic, int topicnum,ofstream &outfile);
  void printMessage(Topic &one_topic, int topicnum);
  void QueryWeiboIntime( std::map<string ,std::list<std::string> >&table_to_weibo,
      std::list<Weibo> &weibocontentlist);
  void SaveFileToLocal(Topic &one_topic,int id);
  std::string intTostring(int a);
};




#endif /* TOPICVIEWANDPOLITICS_H_ */
