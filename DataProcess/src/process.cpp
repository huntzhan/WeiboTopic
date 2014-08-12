/******************************************************************************* 
Filename   : process.cpp
Version    : ver 1.0
Author     :zhuangjf@gmail.com 
Date       : 2014/07/30  
Description:   
History    :
*******************************************************************************/
#include "process.h"

static std::string ICTspilt(const char * sinput);
static void goodWordsinPieceArticle(const std::string &rawtext,std::set<std::string> &stopwords, std::vector<std::string> &goodword);
static void select_time(char * sql_query,char *result);
static std::string RegexReplace(std::string input);
static  std::wstring StringToWide(std::string &sToMatch);
static  std::string WidetoString(std::wstring &wsm);
static struct tm tranformTime(std::string &starttime );

MYSQL my_connection;



//中文字符
#ifdef DEBUG
int main()
{
//	std::set<std::string> stopwordsSet;
//	std::vector<std::string> vec;

// std::cout<<RegexReplace("累死了，累死了@我是米米米米 [泪][泪][泪]还差点死在你手上！！！ 我在:http://t.cn/zYT63A2    你打击嗲   \o(*￣▽￣*)o  *()我是5.5号，JAers，我不一一@你们啦，\o(*￣▽￣*)o  *()我是5.5号，你们懂的～～[爱你]//@毛-叔: @唓哩_Yuen @佳玲Brenda @FLY--ying @西米_ \	__狠爽 @小头米非 @_LIyu 各位，小弟4月底5月初去西藏旅游，如果错过你们的毕业照，请见谅。你们是我大学非经贸学院的好朋友，希望你们能来。你们出了毕业照时间，都告诉我一声啊")<<std::endl;



//	std::string ID="3566050434068596";
//	MakeStopSet(stopwordsSet);
//	Get_StringVector(ID,stopwordsSet,vec);
///	for(std::vector<std::string>::iterator it=vec.begin() ; it!=vec.end();it++){
//				std::cout<<*it<<std::endl;
//	}




	std::set<std::string> stopwords;
	std::vector<std::string> vec;

	std::vector<std::string> IDs;
	MakeStopSet(stopwords);
	std::string ID="2013-04-12 08:38:16";
	char sql[60];
	double total;
	time_t startT,endT;
	init_ICTCAL();



	Get_MIDs("2013-04-12 08:38:16",7400,IDs);
		startT=time(NULL);
		for(std::vector<std::string>::iterator it_ids=IDs.begin();it_ids!=IDs.end();it_ids++){
			ID=*it_ids;
			Get_StringVector(ID,stopwords,vec);
			for(std::vector<std::string>::iterator it=vec.begin() ; it!=vec.end();it++){
					std::cout<<*it<<"|";
				}
			std::cout<<std::endl;

			std::vector<std::vector<std::string> > vecs;
			sprintf(sql,"select text from weibo where mid=%s ",ID.c_str());
			vecs=mysql_query(sql);
			std::cout<<vecs[0][0]<<std::endl;
			vec.clear();
		}
	endT=time(NULL);
	total=difftime(endT,startT);
	std::cout<<"程序运行时间"<<total<<"   num"<<IDs.size()<<std::endl;
/****************************************************************************************************************/

	getchar();
	return 1;
}

#endif
/**************************************************************************************
 * 		分词函数
 */
 static std::string ICTspilt(const char * sinput){
	  std::string result;
	  int nRstLen=0;
	  unsigned int nPaLen;
	  nPaLen=strlen(sinput); // 需要分词的长度
	  if(nPaLen<2){
	 	result="";
	 	return result;
	   }
	  char* sRst=(char *)malloc(1024); //建议长度为字符串长度的倍。
	  if(sRst==NULL)
	    return NULL;
      nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UTF8,0);  //字符串处理
      result=sRst;
      free(sRst);
      sRst=NULL;
      boost::wregex reg(L"\\s+", boost::regex::perl);
      std::wstring wresult=boost::regex_replace(StringToWide(result),reg,std::wstring(L"|"));
      return WidetoString(wresult);
}
 /**************************************************************************************
  * 		初始化中文分词库
  */
void init_ICTCAL(void){
	  if(!ICTCLAS_Init())  {
		    printf("Init fails\n");
			return  ;
		  }
		  else{
		#ifdef DEBUG
			  printf("Init ok\n");
		#endif
		  }
		  //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
		  ICTCLAS_SetPOSmap(2);
		  unsigned int nItems=ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_UTF8);
		  std::cout<<"add "<<nItems<<" user word"<<std::endl;
		  ICTCLAS_SaveTheUsrDic();//保存用户词典
		  mysql_init(&my_connection);
		if (mysql_real_connect(&my_connection, "localhost", "", "", "test", 0, NULL, 0)){
				if ( mysql_set_character_set( & my_connection, "utf8") ) {
				fprintf(stderr,"������ַ� %s \n",mysql_error(&my_connection));
				}
			}
		 else {
				fprintf(stderr, "Connection failed\n");
			    if (mysql_errno(&my_connection))
					{
						fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
					}
			}
}



/*****************************************************************************************
 * 		去噪分割函数   //如果为空  就是空字符""
 */
static void goodWordsinPieceArticle(const std::string &rawtext,std::set<std::string> &stopwords, std::vector<std::string> &goodword){

  std::vector<std::wstring> goodWordstemp;
  bool flag_empty=true;
  if(stopwords.empty())
  {
	  std::cout<<"input goodWordsinPieceArticle error"<<std::endl;
	  return ;
  }
  const std::string temp=RegexReplace(rawtext);  //先正则表达式
  std::string  result=ICTspilt(temp.c_str());
  if(result=="")
  {
	  goodword.push_back(result);
	  return ;
  }
  boost::wregex reg(L"\\d+", boost::regex::perl);  //去掉空格
  std::wstring wtemp=StringToWide(result);
  std::wstring wrtesult=boost::regex_replace(wtemp,reg,std::wstring(L""));
  boost::split(goodWordstemp,wrtesult ,boost::is_any_of("|"));//分割



  for(std::vector<std::wstring>::iterator it=goodWordstemp.begin();it!=goodWordstemp.end();it++){
	  boost::wregex reg3(L"[^0-9a-zA-Z\u4e00-\u9fa5]*|", boost::regex::perl);
	   	std::wstring result=boost::regex_replace(*it,reg3,std::wstring(L""));
	   	std::string temp=WidetoString(result);
	    boost::trim(temp);
	    if(!stopwords.count(temp)&&!temp.empty()){
	    	goodword.push_back(temp);
	    }
	}
  if(goodword.size()<1)
  {
	  goodword.push_back("");
  }

}

/***************************************************************************************************
 * 获得停词
 */
void MakeStopSet(std::set<std::string> &stopwordsSet){
	std::ifstream ifile("stopwords.txt");
	while(!ifile.eof()){
      std::string temp;
	  ifile>>temp;
	  boost::trim(temp);
	  stopwordsSet.insert(temp);
	}
}


std::vector<std::vector<std::string> > mysql_query(char * sql_query){
	int res;
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	std::string m_str;
	std::vector<std::vector<std::string> > result;
	res=mysql_query(&my_connection,sql_query);
	if (res) {
				fprintf(stderr, "SELECT error: %s\n", mysql_error(&my_connection));
	} else {
			res_ptr = mysql_use_result(&my_connection);   //这个函数有问题
			std::vector<std::string> v;
			std::vector< std::vector<std::string> > temp;
			result=temp;
			if (res_ptr) {
				while ( (sqlrow = mysql_fetch_row(res_ptr) ) ){
			        int i=0;
			        int num=0;
			        num=mysql_field_count(&my_connection);
			       while( i<num ){
			    	    if(sqlrow[i]==NULL){
			    		   sqlrow[i]="";
			    	    }
						v.push_back(sqlrow[i]);
						i++;
						}
						result.push_back(v);
						v.clear();
					}
			       	 if (mysql_errno(&my_connection)){
			        	fprintf(stderr, "Retrive error: %s\n", mysql_error(&my_connection));
			        }
				 mysql_free_result(res_ptr);
				 }
		}
	 return result;
}

/**************************************************************************
 * 显示vector二维里面的内容
 */
void display(std::vector< std::vector<std::string> >  &msg)
{
	for(int i=0;i<msg.size();i++)
			for(int j=0;j<msg[i].size();j++)
					std::cout<<msg[i][j]<<std::endl;
}

void Get_MIDs(std::string starttimes, int seconds,std::vector<std::string> &IDs)
{
	char  sql_query[60];
	char  endtime[20];
	std::vector<std::string> MIDs;
	std::vector< std::vector<std::string> > temp;	
	time_t t_of_day;
	struct tm *time;
	struct tm timetmep;
	if(starttimes.empty()){
		printf("____________________get_mids funciton imput error_____________\n");

	}
	timetmep=tranformTime(starttimes);
	time=&timetmep;
	t_of_day=mktime(time)+seconds;	
	time=localtime(&t_of_day);
	sprintf(endtime,"%d-%d-%d %d:%d:%d",time->tm_year+1900,time->tm_mon+1,time->tm_mday,time->tm_hour,time->tm_min,time->tm_sec);
	sprintf(sql_query,"select mid from weibo where weiboDate between '%s' and '%s' ",starttimes.c_str(),endtime);
	temp=mysql_query( sql_query);
	for(std::vector< std::vector<std::string> >::iterator iter=temp.begin();iter!=temp.end();iter++){
		IDs.push_back((*iter)[0]);
	}

}

void Get_StringVector(std::string &mid,std::set<std::string> &stopwordsSet,std::vector<std::string> &vec){
	char  sql_query[60];
	std::vector< std::vector<std::string> > temp;	
	if(mid.empty()){
		printf("____________________Get_StringVector funciton imput error_____________\n");
		return ;
	}
	sprintf(sql_query,"select text from weibo where mid=%s ",mid.c_str());
	temp=mysql_query( sql_query);
	goodWordsinPieceArticle(temp[0][0],stopwordsSet,vec);
}

/*************************************************************************************
 * 		"2014-12-07 12:03:11" ---------------->struct tm
 * 		时间格式转换
 */
static struct tm tranformTime(std::string &starttime ){
	struct tm time;
	int start_pos=0,pos=0;
	std::string temp[6];
	for(int i=5 ;i>=0;i--){
			while(starttime[start_pos]<'0'||starttime[start_pos]>'9'){
				start_pos++;
			}
			pos=start_pos;
			while(starttime[pos]>='0'&&starttime[pos]<='9'){
				pos++;
			}
		temp[i]=starttime.substr(start_pos,pos-start_pos);
		start_pos=pos+1;
	}
	time.tm_sec=atoi(temp[0].c_str());
	time.tm_min=atoi(temp[1].c_str());
	time.tm_hour=atoi(temp[2].c_str());
	time.tm_mday=atoi(temp[3].c_str());
	time.tm_mon=atoi(temp[4].c_str())-1;
	time.tm_year=atoi(temp[5].c_str())-1900;
	return time;
}

 /*************************************************************************************
 * 窄字符转换成宽字符
 */
static std::wstring StringToWide(std::string &sToMatch){
	std::setlocale(LC_ALL,"zh_CN.UTF-8");
	int iWLen=1024;
	wchar_t lpwsz[iWLen];
	 mbstowcs( lpwsz, sToMatch.c_str(), iWLen-2 );
	 lpwsz[iWLen-2]='\0';
	std::wstring wsToMatch(lpwsz);
	//std::setlocale(LC_ALL,"");
	return wsToMatch;
}
  /*************************************************************************************
 * 宽字符转换成窄字符
 */
static std::string WidetoString(std::wstring &wsm){
		std::setlocale(LC_ALL,"zh_CN.UTF-8");
		int iLen=1024;
		char lpsz[iLen];
	    wcstombs( lpsz, wsm.c_str(), iLen-2 ); // 转换。（没有结束符）
	    lpsz[iLen-2] = '\0';
	    std::string sToMatch(lpsz);
	  //  std::setlocale(LC_ALL,"");
	    return sToMatch;
}

 /*************************************************************************************
 * 正则表达式去噪
 * input 需要去噪的句子
 * output 已经去砸的句子
 *  1 表情  如 [高兴]    								 regex：-----------------   \\[([^x00-xff]*)\\]
 *  2 @微博 名         @traa @傅fuuuu @记得我叫青蛙忘了我叫俞映庭:  regex  -------- /?/?\@([^ ]*)[:]|/?/?\@([^ ]*)[ ]   以空格和:结尾
 *  3 超链接    http									 regex -----------------[a-zA-z]+://[^s]*
 *  3 一些特殊的字符              ~@#￥%%……&……                   regex ----------------[()——_*$#]*|【动画】|〜
 */

static std::string RegexReplace(std::string input){
	std::string output;
	std::wstring put;
	if(input.empty())
	{
		 std::cout<<"input RegexReplace error"<<std::endl;
		return NULL;
	}
	//去掉表情
	input.append(" ");
	boost::wregex reg(L"\\[([^x00-xff]*)\\]|/?/?\@([^ ]*)[:]|/?/?\@([^ ]*)[ ]|[a-zA-z]+://[^s]*", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg,std::wstring(L""));
	input=WidetoString(put);
//	boost::wregex reg1(L"[a-zA-z]+://[^s]*", boost::regex::perl);
//	put=boost::regex_replace(StringToWide(input),reg1,std::wstring(L""));
//	input=WidetoString(put);

	//去掉@人名转发     思路去输掉空格                  @非空格+空格
//	boost::wregex reg2(L"/?/?\@([^ ]*)[ ]", boost::regex::perl);
	//匹配非数字英文中文等特殊符号
	boost::wregex reg2(L"【动画】|[()——_*$#┮【】「┭∠」ʑ≦≧▽з؞]*", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg2,std::wstring(L""));
	input=WidetoString(put);
	return input;
}











