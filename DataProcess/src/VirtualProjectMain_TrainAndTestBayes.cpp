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

	 TrainModel tm;
	 tm.TrainClassModel();
	 TestModel testmodel;
	 testmodel.ReadTest();
     return 0;
}
