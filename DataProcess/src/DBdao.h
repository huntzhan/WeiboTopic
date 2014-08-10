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
	std::set<std::string> stop_word_set;
	std::list<string> weibo_id_list;
	std::list<string> k_hours_weibo_id_list;
	void GetCurrentHourWeiboList(std::string time);
	void GetKHourWeiboList(std::string from ,int k);
	void GetEveryWeiboFromDatabase(std::string weiboID,Weibo &oneweibo); //这个函数是用来从数据库边读取前七个小时的微博，避免读取进去内存。
//	void StopWordInit(std::string filename);
	void DBdaoInit();

};


#endif /* DBDAO_H_ */
