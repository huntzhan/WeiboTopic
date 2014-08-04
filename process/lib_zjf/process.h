#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <fstream>
#include "mysql.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <locale>


//#define DEBUG

#include "ICTCLAS50.h"
#include "time.h"

static std::string ICTspilt(const char * sinput);
static std::vector <std::string> goodWordsinPieceArticle(const std::string &rawtext,std::set<std::string> &stopwords, std::vector<std::string> &goodword);


static void select_time(char * sql_query,char *result);
static std::string RegexReplace(std::string input);
static  std::wstring StringToWide(std::string &sToMatch);
static  std::string WidetoString(std::wstring &wsm);

static struct tm tranformTime(std::string &starttime );

/*********************************************************
 * 描述  输入开始时间和时间段   返回该时间段的ID
 * input 开始时间  "2014-11-31 12:10:10"
 *       second   秒数
 * output  这个时间段的微博ID
 * 使用方法
 * 		vector<string> IDs=Get_MIDs("2014-11-31 12:10:10",3600);
 * 		for(vector<string>::iterator it=IDs.begin() ;it!=IDs.end(),it++)
 *
 */

std::vector<std::string> Get_MIDs(std::string starttimes, int seconds=3600);



/**********************************************************
 * 描述  输入ID号 返回分好词的微博内容
 * input   mid   eg :"3566050266282005"  停词表set<string>   vec用来接收分好的词
 *
 *
 * 如   mid="3566050266282005"           微博内容为    中华人民共和国
 * vector[0]=中华
 * vector[1]=人民
 * vector[2]=共和国
 */

void Get_StringVector(std::string &mid,std::set<std::string> &stopwordsSet,std::vector<std::string> &vec);
/************************************************************
 *
 * input   sql_query   eg :"selcet * from weibo "
 *
 * output 	二维的vector  可以调用display来显示
 *
 *    diallay(mysql_query("selcet * from weibo "));
 */

std::vector<std::vector<std::string> > mysql_query(char * sql_query);


void MakeStopSet(std::set<std::string> &stopwordsSet);
void display(std::vector< std::vector<std::string> > &msg);

#endif
