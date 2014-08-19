/*
 * View.h
 *
 *  Created on: 2014年8月11日
 *      Author: hogachen
 */

#ifndef VIEW_H_
#define VIEW_H_
#include<iostream>
#include<string>
#include<vector>

class View{
  //观点主题
	std::string view_sentences;

	//共现重要度
	double view_importance;

	//中间生成的单位关键词组合而成的短语
	std::vector<std::string>temp_view_sentences;
};
#endif /* VIEW_H_ */
