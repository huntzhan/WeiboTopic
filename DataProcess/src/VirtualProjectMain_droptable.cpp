/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 *  description：
 *      先生成一个数据库的操作类，生成特征词（特征词，主题词，关键词都是指话题生成时的词，只是在不同阶段叫法不一样）
 *      再计算特征词 的共现度，最后一趟聚类生成话题
 */
#include"TestModel.h"
#include"TopicViewAndPolitics.h"
#include"TrainModel.h"
#include"DBoperation.h"
#include "GetTopic.h"
#include "Cluster.h"
#include "DEBUG.h"
#include "coverage.h"
#include<iostream>
#include<time.h>
#define DEBUG
#define TIME


int main(int argc, char * argv[]) {

#ifdef TIME
	time_t startmain;
	startmain = time(NULL);
#endif

	//链接数据库的信息
	std::list<std::string> table;
	std::string database_name;
	string topic_table_name;
	ConnPool *connpool=ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 10);
	DBoperation dboper;
	
//	CCoverage m_coverage(&dboper);

  //查询数据库时查询的表的个数
    database_name="split";

	dboper.DBinit(database_name,topic_table_name, connpool);

	std::string drop_table_name="Topic_%";
	dboper.DropTable(drop_table_name);
	drop_table_name="OneDayTopic_%";
	dboper.DropTable(drop_table_name);
#ifdef TIME
	time_t ends6;
	ends6 = time(NULL);
	std::cout << "整个过程用时：" << difftime(ends6, startmain) << " 秒"<<std::endl;
#endif
	return 0;
}
