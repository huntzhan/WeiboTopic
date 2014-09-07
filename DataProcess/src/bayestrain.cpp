/*
 * Main.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen  VirtualProjectMain.cpp
 *  description：
 *		use to train the baye model
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


/*
 *@input param
	argv[1] : normal or other class data directory
	argv[2] : politic or trash class data directory
 */


int main(int argc, char * argv[]) {

	 if(argc!=3){
		 std::cout<<"参数个数错误"<<std::endl;
		 return 0;
	 }
	 std::string otherdir(argv[1]);
	 std::string politicdir(argv[2]);
	 TrainModel tm;
	 tm.TrainClassModel(otherdir,politicdir);
     return 0;
}
