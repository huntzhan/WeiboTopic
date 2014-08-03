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

//�ļ�Ŀ¼ -I /home/jeffzhuang/weibo/software/boost-1_56_0_b1/include/  ͷ�ļ�
//��̬��      /home/jeffzhuang/weibo/software/boost-1_56_0_b1/lib/libboost_regex.a


#include "ICTCLAS50.h"
#include "time.h"

static std::string ICTspilt(const char * sinput);
static std::set<std::string> MakeStopSet(void);
static std::vector <std::string> goodWordsinPieceArticle(const char * rawtext,std::set<std::string> stopwords);

static void display(std::vector< std::vector<std::string> > msg);
static void select_time(char * sql_query,char *result);
static std::string RegexReplace(std::string input);
static  std::wstring StringToWide(std::string sToMatch);
static  std::string WidetoString(std::wstring wsm);

static struct tm tranformTime(std::string starttime );

/**
 * 描述  输入开始时间和时间段   返回该时间段的ID
 * input 开始时间  "2014-11-31 12:10:10"
 *       second   秒数
 * output  这个时间段的微博ID
 */

std::vector<std::string> Get_MIDs(std::string starttimes, int seconds=3600);

/**
 * 描述  输入ID号 返回分好词的微博内容
 * input   mid   eg :"3566050266282005"
 *
 * output  分好词内容
 */

std::vector<std::string > Get_StringVector(std::string mid);

/**
 * 描述  数据库查询语句
 * input   mid   eg :"3566050266282005"
 *
 * output 二维的vector
 */

std::vector< std::vector<std::string> > mysql_query(char * sql_query);

#endif
