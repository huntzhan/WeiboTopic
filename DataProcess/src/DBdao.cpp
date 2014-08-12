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
	void DBdao::GetCurrentHourWeiboList(std::string time,int timeslip){
//		std::string time2="2013-04-12 12:00:00";
		std::vector<string> temp_vec;
		Get_MIDs(time,timeslip,temp_vec);
		std::copy(temp_vec.begin(),temp_vec.end(),std::back_inserter(weibo_id_list));

	}
	void DBdao::GetKHourWeiboList(std::string time ,int timeslip,int k){
//		std::string time2="2013-04-12 23:00:00";
		std::vector<string> temp_vec;
		Get_MIDs(time,ktimeslip,temp_vec);
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
	void DBdao::DBdaoInit(std::string current_time,int timeslip,std::string K_hour_time,int ktimeslip,int hours){
//		StopWordInit(filename);
		init_ICTCAL();
		MakeStopSet(stop_word_set);
		std::cout<<"+++++++++++++++++++++++++++++"<<std::endl;
		GetCurrentHourWeiboList(current_time,timeslip);
		GetKHourWeiboList(K_hour_time,ktimeslip,hours);
	}

	std::string DBdao::GetOriginalWeibo(std::string weiboId){
		char  sql_query[60];
		std::string result;
		std::vector< std::vector<std::string> > temp;
		sprintf(sql_query,"select text from weibo where mid=%s ",weiboId.c_str());
		temp=mysql_query( sql_query);
		result=temp[0][0];
		return result;
	}

