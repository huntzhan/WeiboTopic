/*************************************************************************
	> File Name: TopicSimhash.h
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2014年09月07日 星期日 15时30分11秒
 ************************************************************************/

#include<iostream>
#include"simhash.h"
#include"Topic.h"
class TopicSimhash{
public:
	unsigned int CalTopicWordSimhash(Topic &one_topic,SimHash &simhash); 
	void CalSimHashAndDeleteWeibo(Topic &one_topic);
};
