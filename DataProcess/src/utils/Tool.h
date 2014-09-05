/*************************************************************************
	> File Name: Tool.h
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: Fri 05 Sep 2014 11:01:10 AM CST
 ************************************************************************/

#include<iostream>
#include<string>

class Tool{
public:
	int str2int(std::string str){
		int i=0;
		sscanf(str.c_str(),"%d",&i);
		return i;
	}
	bool IsTwoNumValueable(int from , int to){
		std::cout<<from<<" "<<to<<std::endl;
		if(from<0){
			std::cout<<"表的下标越界"<<std::endl;
			return false;
		}
		if(from>=to){
			std::cout<<"输入表的下标反了"<<std::endl;
			return false;
		}
		
		return true;
	}
};
