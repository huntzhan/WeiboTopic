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
#include <iostream>
namespace hoga{
class DBdao{
	void GetCurrentHourWeiboList(string time);
	void GetKHourWeiboList(string from ,string to);
	Weibo GetEveryWeiboFromDatabase(string weiboID); 			//这个函数是用来从数据库边读取前七个小时的微博，避免读取进去内存。
};
}

#endif /* DBDAO_H_ */
