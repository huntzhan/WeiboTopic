#ifndef _DB_OPERATION_H_
#define _DB_OPERATION_H_
#include "OneWeibo.h"
#include "connection_pool.h"
#include"Weibo.h"
#include"Topic.h"
#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <mysql/mysql.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include<fstream>
namespace Coverage{
	typedef struct _weibo{
		std::string mid;
		std::string content;
	}WEIBO;
}

typedef struct _insert_data {
	std::string mid;
	std::string text;
	std::string spilt;
} INSERT_DATA;

class DBoperation {
public:
	ConnPool *m_connpool;

	Connection *con;
	Statement *state;

	std::string topic_table_name;
	std::string database_name;

	void DB_query(char * sql_query, std::list<std::list<std::string> > &result);

	std::string table_name;
	int weibo_size;
	int OneTimeReadWeiboNum;
	int tableIndex;
	std::vector<std::string> table;
	void ChangeOneTableCount(int & count);
	bool ChangeToNextTable(int & count);
	void SetTableIndexToZero();
	void GetOneWeiBo(std::string table_name1, std::string string,Weibo &oneweibo);
	void DBTableInit(int weibo_size,int OneTimeReadWeiboNum,int tableIndex,std::list<std::string>table);

	MYSQL my_connection;
	void DBinit(std::string database, std::string topic_table_name, ConnPool *connpool);
	void DBclose();
	void SetTableName(const std::string table_name);
	long GetTablecount();
	void GetMidandText(long startline, long length,std::list<OneWeibo> &result);
	void ShowTan();
	void SetAllTable(std::list<std::string> &tables,int from , int to);

	void CreateTable(std::string mytablename);
	int  GetNewserID(std::string OneDayTopic_name);
	void InsertTopicWeiboIdToDatabase(Topic &onetopic,std::string mytablename);

	void InsertData(Topic &onetopic, int flag, ofstream &outfile);
	std::vector<std::string> stringSplitToVector(std::string  &str,int splitLen);

	std::vector<std::string> BoostMatchWord(std::string str);
	std::string Gentime();
	void SelectOnetableByBatch(std::string table, std::list<std::string> &weibolist, std::list<Weibo> &weibo);
	~DBoperation();
	void CreateOneDayTopicTable(std::string mytablename);
	bool QueryIfTableExist(std::string tablename);
	void QueryIfOneDayTableExistAndCreate();
	std::string GenDayAndTime();
	std::vector<std::string> RegexTagWord(std::string &weibo_origin_text);
	bool iswordinregexresult(std::vector<std::string>&regexresult, std::string word);
	void GetMidandTextAndSplit(long startline,long length,std::list<OneWeibo> &result);
	void GetTagText(std::list<Coverage::WEIBO > &result,std::string content);
	void getTopicMainIdea(std::vector<pair<int ,string>  > &result);
	void DropTable(std::string table_prefix);
};

#endif
