/*
 * DBdao.cpp
 *
 *  Created on: 2014年8月4号
 *      Author: hogachen
 */
#include"DBdao.h"
#include<fstream>
#include<iostream>

//    void DBdao::StopWordInit(std::string filename){
//    	std::ifstream infile(filename.c_str());
//    	std::string tempstr;
//    	while(!infile.eof()){
//    		infile>>tempstr;
//    		stop_word_set.insert(tempstr);
//    	}
//    	infile.close();
//    }
//
	void DBdao::GetCurrentHourWeiboList(std::string time){
		std::string time2="2013-04-12 12:00:00";

		process.Get_MIDs(time2,600,weibo_id_list);
//		std::copy(temp_vec.begin(),temp_vec.end(),std::back_inserter());

	}
	void DBdao::GetKHourWeiboList(std::string from ,int k){
		std::string time2="2013-04-12 12:00:00";
		process.Get_MIDs(time2,2000,k_hours_weibo_id_list);
//		std::copy(temp_vec.begin(),temp_vec.end(),std::back_inserter(k_hours_weibo_id_list));
	}
	void DBdao::GetEveryWeiboFromDatabase(std::string weiboID,Weibo &oneweibo){
		 std::list<std::string> mylist;
		 process.Get_Stringlist(weiboID,mylist);
		 std::vector<std::string>vec;
		 std::copy(mylist.begin(),mylist.end(),std::back_inserter(vec));

		 oneweibo.SetWeiboContentWords(vec);
	}
	void DBdao::DBdaoInit(){
//		StopWordInit(filename);
		GetCurrentHourWeiboList("");
		GetKHourWeiboList("",6);
	}

