/*
 * DEBUG.cpp
 *
 *  Created on: 2014年8月6日
 *      Author: hogachen
 *  description：
 *      用来调试时打印信息
 */
#include"Topic.h"
#include"DEBUG.h"
#include"Weibo.h"
//#define BELONT_TOPIC_THROD 1.0
void printVectors(std::list<std::string>&mylist) {
  std::list<std::string>::iterator it = mylist.begin();
  int count = 0;
  for (; it != mylist.end(); ++it) {
    count++;
    if (count > 10)
      break;
    std::cout << *it << std::endl;
  }
}

void printMaps(std::map<std::string, TopicWord> &mymap) {
  std::cout << "topicword size: " << mymap.size() << std::endl;
  std::map<std::string, TopicWord>::iterator it = mymap.begin();
  int count = 0;
  for (; it != mymap.end(); ++it) {
    count++;
    if (count > 10000000)
      break;
    std::cout << "map: " << it->first << "		词频：" << it->second.m_dFrequency
        << std::endl;
  }
}
void printTopic(std::vector<Topic>*clusterlist,int NUM_OF_SUB_WORD) {
  std::vector<Topic>::iterator clit = clusterlist->begin();
  int ti = 0;
  for (; clit != clusterlist->end(); ++clit) {
    ++ti;
    std::list<TopicWord>*topic_vec = clit->GetsTopic();
    std::list<TopicWord>::iterator it = topic_vec->begin();
    std::cout << std::endl << std::endl;
    printTopicView(*clit,NUM_OF_SUB_WORD);
    std::cout << "topic_message_num: " << clit->topic_message_num << endl;
    std::cout << "话题 " << ti << "如下： " << std::endl;
    for (; it != topic_vec->end(); ++it) {
      std::cout << it->GetTopicWord() << "	"; //<<it->GetFrequency()<<"	";
    }
    std::cout << std::endl; //这里以后插入数据库要换成vector来查找
  }
}

void printMatrix(std::map<std::string, CooccurrenceWord> &co_ccur_matrix) {
  std::map<std::string, CooccurrenceWord>::iterator it = co_ccur_matrix.begin();
  for (; it != co_ccur_matrix.end(); ++it) {
    std::cout << it->first << "		word map size is : "
        << it->second.word_cooccurrence.size() << std::endl;
    std::map<std::string, double>::iterator it2 =
        it->second.GetWordCooccurrence()->begin();
    for (; it2 != it->second.GetWordCooccurrence()->end(); ++it2) {
      std::cout << it2->first << "		" << it2->second;
    }
    std::cout << std::endl;
  }
}
void printTopicView(Topic &onetopic,int NUM_OF_SUB_WORD) {

  std::list<subword>*subwordlist = onetopic.GetSubWordList();
  std::list<subword>::iterator it = subwordlist->begin();
  int count = 0;
  std::cout<<std::endl;
  std::cout << "话题概要如下： " << std::endl;
  std::cout << "***************" << std::endl;
  for (; it != subwordlist->end(); ++it) {
    if (++count > NUM_OF_SUB_WORD)
      break;
    std::cout << it->word  << std::endl;
  }
  std::cout << "***************" << std::endl;
}


void printVector(std::vector<PAIR> &sort_vec) {
  std::vector<PAIR>::iterator it = sort_vec.begin();
  for (; it != sort_vec.end(); ++it) {
    std::string word = it->first;
    double value = it->second.m_dFrequency;
    std::cout << word << "		词频：" << value << std::endl;
  }
}
