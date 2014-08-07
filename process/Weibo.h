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

#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Weibo{
	string MID;
	string time;
	vector<string> weibo_content_words;
	double forword;
	double mark;
	double comment;
public:
//	Weibo(vector<string>& weibo_content_words,string & mid){
//		this->MID=mid;
//		this->weibo_content_words=weibo_content_words;
//	}
	vector<string> * GetWords(){
		return & weibo_content_words;
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
	void SetWeiboContentWords(vector<string> &vec){
		this->weibo_content_words=vec;
	}
};

#endif /* MESSAGE_H_ */
