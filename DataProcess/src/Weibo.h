/*
 * Weibo.h
 *
 * Created on: 2014年7月31日
 *      Author: hogachen
 * Discription:
 * 		一条微博
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include"process.h"
#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Weibo{
	string MID;
	string time;
	vector<string> weibo_content_words;
	std::vector<Word> weibo_content_property;
	double forword;
	double mark;
	double comment;
public:
//	Weibo(vector<string>& weibo_content_words,string & mid){
//		this->MID=mid;
//		this->weibo_content_words=weibo_content_words;
//	}
	vector<Word> * GetContentWithProperty(){
		return &this->weibo_content_property;
	}
	vector<string> * GetWords(){
		return  &weibo_content_words;
	}
	double GetForword(){
		return this->forword;
	}
	double GetMark(){
		return this->mark;
	}
	double GetComment(){
		return this->comment;
	}
	void SetWeiboContentWords(vector<string> &vec,std::string MID){
		this->weibo_content_words=vec;
		this->MID=MID;
	}
	void SetWeiboContentWords2(std::string MID,vector<Word> &vec) {
		this->weibo_content_property = vec;
		this->MID = MID;
	}
};

#endif /* MESSAGE_H_ */
