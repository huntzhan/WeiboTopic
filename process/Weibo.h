/*
 * Weibo.h
 *
 * Created on: 2014��7��31��
 *      Author: hogachen
 * Discription:
 * 		һ��΢��
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
};

#endif /* MESSAGE_H_ */
