/*
 * DEBUG.h
 *
 *  Created on: 2014年8月6日
 *      Author: hogachen
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include"GetTopic.h"
#include"CooccurrenceWord.h"
#include"Topic.h"
#include"DBdao.h"
#include<iostream>
#include<string>
#include<list>
#include<map>

void printVectors(std::list<std::string>&list);
void printMaps(std::map<std::string,TopicWord> &map);
void printTopic(std::vector<Topic>*clusterlist,DBdao* dbdao,int BELONG_TOPIC_THROD);
void printMatrix(std::map<std::string,CooccurrenceWord> &co_ccur_matrix);
void printTopicView(Topic &onetopic);
void printVector(std::vector<PAIR>&sort_vec);
#endif /* DEBUG_H_ */
