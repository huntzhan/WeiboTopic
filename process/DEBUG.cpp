/*
 * DEBUG.cpp
 *
 *  Created on: 2014年8月6日
 *      Author: hogachen
 */

#include"DEBUG.h"
void printVectors(std::list<std::string>&mylist){
	std::list<std::string>::iterator it = mylist.begin();
	int count=0;
	for(;it!=mylist.end();++it){
		count++;
		if(count>10)break;
		std::cout<<*it<<std::endl;
	}
}

void printMaps(std::map<std::string,double> &mymap){
	std::cout<<mymap.size()<<std::endl;
	std::map<std::string,double>::iterator it=mymap.begin();
	int count=0;
	for(;it!=mymap.end();++it){
		count++;
		if(count>100)break;
		std::cout<<it->first<<"		"<<it->second<<std::endl;
	}
}
