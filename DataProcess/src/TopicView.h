/*
 * TopicView.h
 *
 *  Created on: 2014年8月11日
 *      Author: hogachen
 *  输入：
 *  	一个微博话题下的所有微博ID和这个话题的主要关键词
 *  输出：
 *  	话题主要观点topN,数据存储格式list<view>
 *  过程：提取词性标注后的形容词，同时提取该词周围的部分词来生成短语，可以考虑利用观点词前后3个词的距离来生成单位关键词，最后统计词频最高的作为摘要。通过计算与话题的共现度来排序
 *
 */

#ifndef POINT_H_
#define POINT_H_
#include"View.h"
#include"DBdao.h"
#include"Topic.h"
#include"CooccurrenceWord.h"
#include<iostream>
#include<map>
#include<string>
typedef pair<std::string,double> PAIRS;
bool SubTopicSort(const PAIRS & key1,const PAIRS & key2);
class TopicView{
	int NUM_OF_SUB_WORD;
	std::vector<Topic> *clusterList;//最终生成的话题列表
	std::map<std::string,CooccurrenceWord> *co_ccur_matrix;
	DBdao *dbdao;
public:
	void InitTopicView(int NUM_OF_SUB_WORD){
		this->NUM_OF_SUB_WORD=NUM_OF_SUB_WORD;
	}
	TopicView(DBdao *dbdao,std::vector<Topic> *clusterList,
				std::map<std::string,CooccurrenceWord> *co_ccur_matrix){
		this->clusterList=clusterList;
		this->co_ccur_matrix=co_ccur_matrix;
		this->dbdao=dbdao;
	}
	void GenOneTopicView(Topic & onetopic);

	void SortSubTopicMap(Topic &onetopic);
	void GenAllTopicView();
};
#endif /* POINT_H_ */
