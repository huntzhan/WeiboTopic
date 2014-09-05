/*
 * DEBUG.h
 *
 *  Created on: 2014年8月6日
 *      Author: hogachen
 *  description：
 *      用来调试时打印信息
 */

#ifndef DEBUG_H_
#define DEBUG_H_
#include"DBoperation.h"
#include"GetTopic.h"
#include"CooccurrenceWord.h"
#include"Topic.h"
#include<iostream>
#include<string>
#include<list>
#include<map>

void printVectors(std::list<std::string>&list);
void printMaps(std::map<std::string,TopicWord> &map);
void printTopic(std::vector<Topic>*clusterlist,int NUM_OF_SUB_WORD);
void printMatrix(std::map<std::string,CooccurrenceWord> &co_ccur_matrix);
void printTopicView(Topic &onetopic,int NUM_OF_SUB_WORD);
void printVector(std::vector<PAIR>&sort_vec);
#endif /* DEBUG_H_ */