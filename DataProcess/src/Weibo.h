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

//	vector<Word> * GetContentWithProperty() {
//		return &this->weibo_content_property;
//	}
//	vector<string> * GetWords() {
//		return &weibo_content_words;
//	}
	double GetForword() {
		return this->forword;
	}
	double GetMark() {
		return this->mark;
	}
	double GetComment() {
		return this->comment;
	}

	void SetWeiboContentWords(std::string &spilt, std::string &text) {
		this->spilt = spilt;
		this->text = text;
	}

};

#endif /* MESSAGE_H_ */
