/*
 * Weibo.h
 *
 * Created on: 2014年7月31日
 *      Author: hogachen
 * Discription:
 * 		一条原始的未处理的微博，区别于OneWeibo.h中的已经处理好的微博
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_
#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Weibo {
public:
	string belongtable;
	string time;
	std::string mid;
	std::string text;
	std::string spilt;
	double forword;
	double mark;
	double comment;
	unsigned int simhashvalue;
	int topic_simhash_distance;

	double GetForword() {
		return this->forword;
	}
	double GetMark() {
		return this->mark;
	}
	double GetComment() {
		return this->comment;
	}
	std::string GetText(){
	  return this->text;
	}
	std::string GetSpilt(){
	  return this->spilt;
	}
	void SetWeiboContentWords(std::string &spilt, std::string &text) {
		this->spilt = spilt;
		this->text = text;
	}

};

#endif /* MESSAGE_H_ */
