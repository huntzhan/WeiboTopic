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
		std::vector<string> temp_vec;
		Get_MIDs(time2,36000,temp_vec);
		std::copy(temp_vec.begin(),temp_vec.end(),std::back_inserter(weibo_id_list));

	}
	void DBdao::GetKHourWeiboList(std::string from ,int k){
		std::string time2="2013-04-12 22:00:00";
		std::vector<string> temp_vec;
		Get_MIDs(time2,360000,temp_vec);
		std::copy(temp_vec.begin(),temp_vec.end(),std::back_inserter(k_hours_weibo_id_list));
	}
	void DBdao::GetEveryWeiboFromDatabase(std::string weiboID,Weibo &oneweibo){
		 std::vector<std::string> mylist;
		// MakeStopSet(stop_word_set);
		 Get_StringVector(weiboID,stop_word_set,mylist);
	//	 std::vector<std::string>vec;
	//	 std::copy(mylist.begin(),mylist.end(),std::back_inserter(vec));

		 oneweibo.SetWeiboContentWords(mylist,weiboID);
	}
	void DBdao::DBdaoInit(){
//		StopWordInit(filename);
		init_ICTCAL();
		MakeStopSet(stop_word_set);
		std::cout<<"+++++++++++++++++++++++++++++"<<std::endl;
		GetCurrentHourWeiboList("");
		GetKHourWeiboList("",6);
	}

