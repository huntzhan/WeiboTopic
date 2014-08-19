/*
 * IsPolitic.h
 *
 *  Created on: 2014年8月12日
 *      Author: hogachen
 *  description：
 *      政治类话题提取
 */

#ifndef ISPOLITIC_H_
#define ISPOLITIC_H_
#include<iostream>
#include<string>
class IsPolitics{
  //最终生成的话题列表
	std::vector<Topic> *clusterList;
	std::vector<Topic*> politic_cluster_list;
	DBdao *dbdao;
public:
	IsPolitics(DBdao *dbdao,std::vector<Topic> *clusterList);
	void IsOneTopicPolitic(Topic & onetopic);
	void IsAllTopicPolitic();
};

#endif /* ISPOLITIC_H_ */
