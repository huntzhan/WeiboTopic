/*
 * DBpool.h
 *
 *  Created on: 2014年8月17日
 *      Author: jeffzhuang
 */

#ifndef DBPOOL_H_
#define DBPOOL_H_
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <mysql/mysql.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "connection_pool.h"
#include "DBoperation.h"
class DBpool {
private:
	ConnPool *m_connpool;
	Connection *con;
	Statement *state;

	std::string table_name;
	std::string mysql_addr;
	std::string mysql_usr;
	std::string mysql_pwd;
	std::string mysql_database;
public:
	void DBinit(std::string database, ConnPool *connpool);
	void DBclose();
	void SetTableName(const std::string table_name);
	long GetTablecount();
	void GetTables(std::list<std::string> &tables);
	~DBpool();
	void CreateTable();
	bool DB_insertData(std::vector<INSERT_DATA> &insert_data);
	void GetText(long startline, long length,
			std::list<std::list<std::string> > &result);
	void ShowTable(std::list<std::string> &tables);
};
#endif /* DBPOOL_H_ */
