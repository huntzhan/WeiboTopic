/*
 * DBdao.h
 *
 *  Created on: 2014年8月4日
 *      Author: hogachen
 */

#ifndef DBDAO_H_
#define DBDAO_H_
#include "Weibo.h"
#include "process.h"
#include <string>
#include <list>
#include <set>
#include <iostream>

class DBdao{
public:
	std::string current_time;//格式："2013-04-12 12:00:00"
	std::string K_hours_time;
	int    hours;
	std::set<std::string> stop_word_set;
	std::list<string> weibo_id_list;
	std::list<string> k_hours_weibo_id_list;
	void GetCurrentHourWeiboList(std::string time,int timeslip);
	void GetEveryWeiboFromDatabase(std::string weiboID,Weibo &oneweibo); //这个函数是用来从数据库边读取前七个小时的微博，避免读取进去内存。
	std::string GetOriginalWeibo(std::string weiboId);
	void GetKHourWeiboList(std::string time ,int timeslip,int k);
	void DBdaoInit(std::string current_time,int timeslip,std::string K_hour_time,int ktimeslip,int hours);
	void GetEveryWeiboWithProperty(std::string mid,Weibo &weibo);

};


#endif /* DBDAO_H_ */
