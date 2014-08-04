/******************************************************************************* 
Filename   : process.cpp
Version    : ver 1.0
Author     :zhuangjf@gmail.com 
Date       : 2014/07/30  
Description:   
History    :
*******************************************************************************/
#include "process.h"




//中文字符
#ifdef DEBUG
int main()
{
	std::set<std::string> stopwordsSet;

// std::cout<<RegexReplace("累死了，累死了@我是米米米米 [泪][泪][泪]还差点死在你手上！！！ 我在:http://t.cn/zYT63A2    你打击嗲   \o(*￣▽￣*)o  *()我是5.5号，JAers，我不一一@你们啦，\o(*￣▽￣*)o  *()我是5.5号，你们懂的～～[爱你]//@毛-叔: @唓哩_Yuen @佳玲Brenda @FLY--ying @西米_ \	__狠爽 @小头米非 @_LIyu 各位，小弟4月底5月初去西藏旅游，如果错过你们的毕业照，请见谅。你们是我大学非经贸学院的好朋友，希望你们能来。你们出了毕业照时间，都告诉我一声啊")<<std::endl;


//	MakeStopSet(stopwordsSet);
/*	Get_MIDs("2013-04-12 00:01:41",120);*/
	std::string ID="3566051122059043";
	Get_StringVector(ID,stopwordsSet);

	getchar();
	return 1;
}

#endif
/**
 * 		分词函数
 */
static std::string ICTspilt(const char * sinput){

  std::string result;
  //初始化
  if(!ICTCLAS_Init())  {

    printf("Init fails\n");
	return NULL ;
  }
  else{
#ifdef DEBUG
    printf("Init ok\n");
#endif
  }
  //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
  ICTCLAS_SetPOSmap(2);

  unsigned int nPaLen=strlen(sinput); // 需要分词的长度
  char* sRst=0;   //用户自行分配空间，用于保存结果；
  sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
  int nRstLen=0; //分词结果的长度

  unsigned int nItems=ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_GB);
  ICTCLAS_SaveTheUsrDic();//保存用户词典


  nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //字符串处理
  result=sRst;
  free(sRst);
  ICTCLAS_Exit();	//释放资源退?
  return result;
}


/*****************************************************************************************
 * 		去噪分割函数
 */
static std::vector <std::string> goodWordsinPieceArticle(const std::string &rawtext,std::set<std::string> &stopwords, std::vector<std::string> &goodword){

  std::vector<std::string> goodWordstemp;
	//= ICTspilt(rawtext);
//	boost::wregex wreg(L"\\d+");//去掉中文的空格
  const std::string temp=RegexReplace(rawtext);  //先正则表达式
  std::string  result=ICTspilt(temp.c_str());
  boost::split(goodWordstemp,result ,boost::is_any_of(" "));

  for(std::vector<std::string>::iterator it=goodWordstemp.begin();it!=goodWordstemp.end();it++){
    std::string temp=*it;
    boost::trim(temp);
    if(!stopwords.count(temp)&&!temp.empty()){
       goodword.push_back(temp);
	  }
	}
	return goodword;
}

/***************************************************************************************************
 * 获得停词
 */
void MakeStopSet(std::set<std::string> &stopwordsSet){
	std::ifstream ifile("stopwords.txt");
	while(!ifile.eof()){
		
      std::string temp;
		//getline(ifile,temp);	
	  ifile>>temp;
	  boost::trim(temp);
	  stopwordsSet.insert(temp);
	}

}


 std::vector<std::vector<std::string> > mysql_query(char * sql_query){
	 int res;
	 MYSQL_RES *res_ptr;
	 MYSQL_ROW sqlrow;
	 MYSQL my_connection; 
	 std::string m_str;
	 std::vector<std::vector<std::string> > result;
	 mysql_init(&my_connection);     
	 if (mysql_real_connect(&my_connection, "localhost", "", "", "test", 0, NULL, 0)){


				
				if ( mysql_set_character_set( & my_connection, "utf8") ) {
					fprintf(stderr,"������ַ� %s \n",mysql_error(&my_connection));
				}
				res=mysql_query(&my_connection,sql_query);
				if (res) {
						fprintf(stderr, "SELECT error: %s\n", mysql_error(&my_connection));   
					} else {    
							/*		if(result.empty())
								{
									printf("empty\n");
								}  */
								res_ptr = mysql_use_result(&my_connection);   //这个函数有问题
						/*		if(result.empty())
								{
									printf("empty\n");
								}  */
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
				mysql_close(&my_connection);
		} 
	 else {    
			fprintf(stderr, "Connection failed\n");   
	    	if (mysql_errno(&my_connection)) 
				{    
					fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection)); 
				}  
		  }

	 return result;
}

/**************************************************************************
 * 显示vector二维里面的内容
 */
void display(std::vector< std::vector<std::string> > &msg)
{
	
	for(int i=0;i<msg.size();i++){
			for(int j=0;j<msg[i].size();j++){
					std::cout<<msg[i][j]<<std::endl;
				}
		}
}

std::vector<std::string> Get_MIDs(std::string &starttimes, int seconds)
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
		return MIDs ;
	}
	timetmep=tranformTime(starttimes);
	time=&timetmep;
	t_of_day=mktime(time)+seconds;	
	time=localtime(&t_of_day);
	sprintf(endtime,"%d-%d-%d %d:%d:%d",time->tm_year+1900,time->tm_mon+1,time->tm_mday,time->tm_hour,time->tm_min,time->tm_sec);
	sprintf(sql_query,"select mid from weibo where weiboDate between '%s' and '%s' ",starttimes.c_str(),endtime);
	temp=mysql_query( sql_query);

	for(std::vector< std::vector<std::string> >::iterator iter=temp.begin();iter!=temp.end();iter++){
			MIDs.push_back((*iter)[0]);
	}
	return MIDs;
	
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



/**********************************************************
 * 		"2014-12-07 12:03:11" ---------------->struct tm
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


static  std::wstring StringToWide(std::string &sToMatch){

	std::setlocale(LC_ALL,"zh_CN.UTF-8");

	int iWLen= mbstowcs( NULL, sToMatch.c_str(), sToMatch.length() );
	wchar_t *lpwsz= new wchar_t[iWLen+1];
	int i= mbstowcs( lpwsz, sToMatch.c_str(), sToMatch.length() );
	std::wstring wsToMatch(lpwsz);
	std::setlocale(LC_ALL,"");
	delete []lpwsz;
	return wsToMatch;
}

static std::string WidetoString(std::wstring &wsm){

		std::setlocale(LC_ALL,"zh_CN.UTF-8");
		int iLen= wcstombs( NULL, wsm.c_str(), 0 ); // 计算转换后字符串的长度。（不包含字符串结束符）
	    char *lpsz= new char[iLen+1];
	    int i= wcstombs( lpsz, wsm.c_str(), iLen ); // 转换。（没有结束符）
	    lpsz[iLen] = '\0';
	    std::string sToMatch(lpsz);
	    std::setlocale(LC_ALL,"");
	    delete []lpsz;
	    return sToMatch;

}



static std::string RegexReplace(std::string input)
{
	std::string output;
	std::wstring put;
	//去掉表情
	boost::wregex reg(L"\\[([^x00-xff]*)\\]|/?/?\@([^ ]*)[ ]|[a-zA-z]+://[^s]* ", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg,std::wstring(L""));
	input=WidetoString(put);


	//去掉@人名转发     思路去输掉空格                  @非空格+空格
//	boost::wregex reg2(L"/?/?\@([^ ]*)[ ]", boost::regex::perl);
	//匹配非数字英文中文等特殊符号
	boost::wregex reg2(L"[()——_*$#]*", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg2,std::wstring(L""));
	input=WidetoString(put);

	return input;
	
}











