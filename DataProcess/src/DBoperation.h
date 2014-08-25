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
	void DBinit(std::string database, ConnPool *connpool);
	void DBclose();
	void SetTableName(const std::string table_name);
	long GetTablecount();
	void GetMidandText(long startline, long length,std::list<OneWeibo> &result);
	void ShowTan();
	void ShowTable(std::list<std::string> &tables);

	void CreateTable(std::string mytablename);
	int  GetNewserID();
	void InsertTopicWeiboIdToDatabase(Topic &onetopic,std::string mytablename);

	void InsertData(Topic &onetopic, int flag);
	std::vector<std::string> stringSplitToVector(std::string  &str,int splitLen);

	std::vector<std::string> BoostMatchWord(std::string str);
	std::string Gentime();
	~DBoperation();
};

#endif
