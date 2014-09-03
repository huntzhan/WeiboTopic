/*
 * coverage.h
 *
 *  Created on: 2014-8-25
 *      Author: jeffzhuang
 */

#ifndef COVERAGE_H_
#define COVERAGE_H_
#include "DBoperation.h"
#include <iostream>
#include <map>
#include "simhash.h"
#include <iomanip>
#include "time.h"
#include "stdio.h"
typedef struct _topic{
	std::string tag;
	int num;
	std::list<std::string> mids;
}TopicTag;

typedef struct _maintopic{
	int id;
	string matchstring;
	vector<string> subTopic;
}MainTopic;

typedef struct _topcicount{
	string hottopic;
	long count;
	vector<pair<string ,long> > topic;
}TopicCount;



class CCoverage{
	DBoperation *dboper;
    std::map<std::string,TopicTag> topics;
    vector<pair<string,TopicTag> > SortVector;
    std::vector<std::string> FilterTable;
    std::vector<std::string> TopicTable;
    std::string OneDayTopic;
    vector<pair<string,unsigned> > TopicHash;
    map<int,MainTopic> matchTopic;
    vector<TopicCount> TopicSearchCount;
    unsigned  int disCount;
    SimHash sim;
    double JaccardThrethod;
    int SimhashThrethod;
public:
    map<unsigned,string> TagHash;  //标准的话题
    vector<MainTopic> topicMainIdea; //聚类出的话题
public:
    void GetTable( std::list<std::string> &table);
    CCoverage(DBoperation *m_dboper){

    	// std::cout<<TagHash.size()<<"|"<<topicMainIdea.size()<<endl;
    	TextSpilt::init_ICTCAL();
    	SimhashThrethod=3;
    	disCount=100;
    	JaccardThrethod=0.5;
    	this->dboper=m_dboper;
    }
    ~CCoverage(){}
    void MatchTopicByJaccard(const std::string hotTopic,const std::string OneDayTopic);
    void MatchTopicBySimHash();
    void GetSearchCount(const std::string tablename);

private:
    void GetTag();
    void SortTag();
    void GetTopicMainIdea(const std::string tablename);
    void MatchTopic();
    double Jaccard(std::string str1,std::string str2);
    void _MatchTopicByJaccard();
    void DisplayMatch();
    void GetHourTopic(const char * path);
    void OutPutResult();
    string TramTime(const std::string table);
    void mGetSearchCount(void);
    void SearchCountSort();
 //  bool SortCmp(const pair<string,TopicTag> &key1,const pair<string,TopicTag> &key2);

};



#endif /* COVERAGE_H_ */
