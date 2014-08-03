/******************************************************************************* 
Filename   : process.cpp
Version    : ver 1.0
Author     :zhuangjf@gmail.com 
Date       : 2014/07/30  
Description:   
History    :
*******************************************************************************/
#include "process.h"



/*
//中文字符

int main()
{
	
/*	std::vector< std::vector<std::string> > result;
	std::vector<std::string> v;

	
	result=mysql_query("select * from weibo limit 3");
	display(result);
	
	std::set<std::string> stopwords;
	std::ofstream fout("out.text");
	std::string output= ICTspilt(result[1][3].c_str());
	std::cout<<output<<std::endl;
	goodWordsinPieceArticle(result[1][3].c_str(),stopwords);	*/


	//std::cout<<result[0][0]<<std::endl;
/*	time_t t_of_day;
	struct tm *time;
	struct tm timetmep;
	timetmep=tranformTime("2014-12-07    12:3:11");
	time=&timetmep;
	t_of_day=mktime(time)+10;	
	time=localtime(&t_of_day);
	printf("%d-%d-%d %d:%d:%d",time->tm_year+1900,time->tm_mon+1,time->tm_mday,time->tm_hour,time->tm_min,time->tm_sec);
*/
//	RegexReplace("[[中文标签]], [你好], inijsa, colourize");
	


/*	Get_MIDs("2013-04-12 00:01:41",120);
	Get_StringVector("3566050391654043");
	//printf("%d-%d-%d %d:%d:%d",time.tm_year,time.tm_mon,time.tm_mday,time.tm_hour,time.tm_min,time.tm_sec);
	getchar();
	return 1;
}*/


/**
 * 		分词函数
 */
static std::string ICTspilt(const char * sinput)
{
	std::string result;
	if(!ICTCLAS_Init()) //��ʼ���ִ������
	{
		printf("Init fails\n");  
		return NULL ;
	}
	else
	{
		printf("Init ok\n");
	}
	//设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
		ICTCLAS_SetPOSmap(2);


		unsigned int nPaLen=strlen(sinput); // 需要分词的长度
		char* sRst=0;   //用户自行分配空间，用于保存结果；
		sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
		int nRstLen=0; //分词结果的长度

		unsigned int nItems=ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_GB);
		ICTCLAS_SaveTheUsrDic();//保存用户词典
		printf("%d user-defined lexical entries added!\n",nItems);



		nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //字符串处理
		result=sRst;



		free(sRst);
		ICTCLAS_Exit();	//释放资源退?
		return result;
}


/**
 * 		去噪分割函数
 */
static std::vector <std::string> goodWordsinPieceArticle(const char * rawtext,std::set<std::string> stopwords)
{
	std::vector<std::string> goodWordstemp;
	std::vector<std::string> goodword;
	//= ICTspilt(rawtext);
//	boost::wregex wreg(L"\\d+");//去掉中文的空格
	std::string sResult	=RegexReplace(ICTspilt(rawtext));
	boost::split(goodWordstemp,sResult,boost::is_any_of(" "));

	for(std::vector<std::string>::iterator it=goodWordstemp.begin();it!=goodWordstemp.end();it++)
	{
		std::string temp=*it;
		boost::trim(temp);
		if(!stopwords.count(temp)&&!temp.empty())
		{
			goodword.push_back(temp);
			std::cout<<temp<<std::endl;
		}
		
	}

	
	return goodword;
}

/**
 * 获得停词
 */
static std::set<std::string> MakeStopSet(void)
{
	std::set<std::string> stopwordsSet;
	std::ifstream ifile("stopwords.txt");
	while(!ifile.eof())
	{
		
		std::string temp;
		//getline(ifile,temp);	
		ifile>>temp;
		boost::trim(temp);
		stopwordsSet.insert(temp);
		
	}
	return stopwordsSet;
}


 std::vector< std::vector<std::string> > mysql_query(char * sql_query)
{
	 int res;
	 MYSQL_RES *res_ptr;
	 MYSQL_ROW sqlrow;
	 MYSQL my_connection; 
	 std::string m_str;
	 std::vector< std::vector<std::string> > result;
	 

	 mysql_init(&my_connection);     
	 if (mysql_real_connect(&my_connection, "localhost", "", "", "test", 0, NULL, 0))
	 	{     
	 			printf("Connection success\n");    
				
				if ( mysql_set_character_set( & my_connection, "utf8") ) 
				{
					fprintf(stderr,"������ַ� %s \n",mysql_error(&my_connection));
				}
				res=mysql_query(&my_connection,sql_query);
				
				if (res) 
					{        
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
						
								if (res_ptr) 
									{     

										 
										while ( (sqlrow = mysql_fetch_row(res_ptr) ) )
										{        
											int i=0;
											int num=0;
											num=mysql_field_count(&my_connection);
											
											while( i<num )
											{
												if(sqlrow[i]==NULL)
												{
													sqlrow[i]="";
												}
												//printf("????%s",sqlrow[i]);
												v.push_back(sqlrow[i]);	
												i++;
											}
											
											result.push_back(v);
											v.clear();
										}          
										
										if (mysql_errno(&my_connection))
											{   
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

/**
 * 显示vector二维里面的内容
 */
static void display(std::vector< std::vector<std::string> > msg)
{
	
	for(int i=0;i<msg.size();i++)
		{
			for(int j=0;j<msg[i].size();j++)
				{
					std::cout<<msg[i][j]<<std::endl;
				}
		}

}

std::vector<std::string> Get_MIDs(std::string starttimes, int seconds)
{
	
	char  sql_query[60];
	char  endtime[20];
	std::vector<std::string> MIDs;
	std::vector< std::vector<std::string> > temp;	
	time_t t_of_day;
	struct tm *time;
	struct tm timetmep;

	if(starttimes.empty())
	{
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
	
	//display(temp);

	
	for(std::vector< std::vector<std::string> >::iterator iter=temp.begin();iter!=temp.end();iter++)
	{
			MIDs.push_back((*iter)[0]);
	}
	
	


	return MIDs;
	
}

std::vector<std::string > Get_StringVector(std::string mid)
{
	char  sql_query[60];
	std::set<std::string> stopwords;
	std::vector<std::string> vec;
	std::vector< std::vector<std::string> > temp;	

	if(mid.empty())
	{
		printf("____________________Get_StringVector funciton imput error_____________\n");
		return vec ;
	}

	
	sprintf(sql_query,"select text from weibo where mid=%s ",mid.c_str());
	std::cout<<sql_query<<std::endl;
	temp=mysql_query( sql_query);
	stopwords=MakeStopSet();
	goodWordsinPieceArticle(temp[0][0].c_str(),stopwords);
	return vec;
}



/**
 * 		"2014-12-07 12:03:11" ---------------->struct tm
 */
static struct tm tranformTime(std::string starttime )
{
	struct tm time;
	int start_pos=0,pos=0;
	std::string temp[6];
	
	for(int i=5 ;i>=0;i--)
		{
			
			
			while(starttime[start_pos]<'0'||starttime[start_pos]>'9')
			{
				start_pos++;
			}
			pos=start_pos;
			while(starttime[pos]>='0'&&starttime[pos]<='9')
			{				
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


static  std::wstring StringToWide(std::string sToMatch)
	{


	
//	std::cout<<std::setlocale(LC_ALL,NULL)<<std::endl;
//	std::locale loc("zh_CN.UTF-8");
//	std::locale::global(loc);
//	int iWLen= mbstowcs( NULL, sToMatch.c_str(), sToMatch.length() ); // ����ת������ַ����ĳ��ȡ����������ַ�����������
//	wchar_t *lpwsz= new wchar_t[255];
////	 int i= mbstowcs( lpwsz, sToMatch.c_str(), sToMatch.length() ); // ת������ת������ַ����н�������
//	std::wstring wsToMatch(lpwsz);
	//printf()
	//wprintf(L"nihao");
	//std::wcout.imbue(std::locale("zh_CN.UTF-8"));
	//std::wcout.imbue(locale(locale(),"",LC_CTYPE));
//	std::cout<<"大家好"<<wsToMatch<<std::endl;


//	std::ios::sync_with_stdio(false);
//	std::locale::global(std::locale("zh_CN.UTF-8"));
//	std::wcout.imbue(std::locale("zh_CN.UTF-8"));
	std::cout<<std::setlocale(LC_ALL,"zh_CN.UTF-8")<<std::endl;

	int iWLen= mbstowcs( NULL, sToMatch.c_str(), sToMatch.length() ); // ����ת������ַ����ĳ��ȡ����������ַ�����������
	wchar_t *lpwsz= new wchar_t[iWLen+1];
	int i= mbstowcs( lpwsz, sToMatch.c_str(), sToMatch.length() ); // ת������ת������ַ����н�������
	std::wstring wsToMatch(lpwsz);


	 std::setlocale(LC_ALL,"");

	
	
	 delete []lpwsz;
	 return wsToMatch;
}

static std::string WidetoString(std::wstring wsm)
{

		std::setlocale(LC_ALL,"zh_CN.UTF-8");
		int iLen= wcstombs( NULL, wsm.c_str(), 0 ); // 计算转换后字符串的长度。（不包含字符串结束符）
	    char *lpsz= new char[iLen+1];
	    int i= wcstombs( lpsz, wsm.c_str(), iLen ); // 转换。（没有结束符）
	    lpsz[iLen] = '\0';
	    std::string sToMatch(lpsz);
	    std::cout<<sToMatch<<std::endl;
	    std::cout<<" pppppp \n"<<std::endl;

	    std::setlocale(LC_ALL,"");

	    delete []lpsz;
	    return sToMatch;

}



static std::string RegexReplace(std::string input)
{
	std::string output;
	std::wstring put;
	boost::wregex reg(L"\\[([^x00-xff]*)\\]", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg,std::wstring(L""));
	return WidetoString(put);;
	
}











