#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <set>
#include <list>
#include <fstream>
#include <list>
#include "mysql/mysql.h"
#include <locale>


//#define DEBUG

#include "src/ICTCLAS50.h"
#include "time.h"


class Process{
private:
	   std::set<std::string> stopwordsSet;
	   std::list< std::list<std::string> > result;
	   char  sql_query[150];
	   std::string  rawtext;
	   std::list<std::string> re;
	   MYSQL my_connection;
public:
	Process();
	~Process(){
		// mysql_close(&my_connection);
		 ICTCLAS_Exit();	//释放资源退?
	};
	/*********************************************************
	 * 描述  输入开始时间和时间段   返回该时间段的IDs
	 * input 开始时间  "2014-11-31 12:10:10"
	 *       second   秒数
	 *       这个时间段的微博ID
	 * 使用方法
	 *		Process process;
	 * 		std::list<std::string> IDs;
	 * 		process.Get_MIDs("2013-04-12 08:38:16",1600,IDs);
	 *		IDs[0]="3566050434068596"
	 */
	void Get_MIDs(std::string starttimes, int seconds,std::list<std::string> &MIDs);
	/**********************************************************
	 * 描述  输入ID号 返回分好词的微博内容
	 * input   mid   eg :"3566050266282005"  停词表set<string>   vec用来接收分好的词
	 *
	 *					std::string ID="3566050266282005"
	 * 					process.Get_Stringlist(ID,vec);
	 * list[0]=中华
	 * list[1]=人民
	 * list[2]=共和国
	 */
	void Get_Stringlist(std::string &mid,std::list<std::string> &vec);
	void Query_DB();
	void display(std::list< std::list<std::string> > &msg);
    std::list< std::list<std::string> >  Get_result();
    void set_mysql_query(const char *);

private:
	void MakeStopSet(const char * path);
	void goodWordsinPieceArticle(std::list<std::string> &goodword);
	std::string WidetoString(std::wstring &wsm);
	std::wstring StringToWide(std::string &sToMatch);
	std::string Regex_Replace(std::string input);
	struct tm tranformTime(std::string &starttime );
	std::string ICTspilt(const char * sinput);
};


#endif
