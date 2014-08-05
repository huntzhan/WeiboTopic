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
	std::list<std::string> IDs;
	std::list<std::string> vec;
	Process process;
	std::string ID="3566050434068596";



	//MakeStopSet(stopwordsSet);
	process.Get_Stringlist(ID,vec);
	process.Get_MIDs("2013-04-12 08:38:16",1600,IDs);


	for(std::list<std::string>::iterator it=vec.begin() ; it!=vec.end();it++){
				std::cout<<*it<<std::endl;
	}


	for(std::list<std::string>::iterator it_ids=IDs.begin();it_ids!=IDs.end();it_ids++){
			ID=*it_ids;
			process.Get_Stringlist(ID,vec);
			for(std::list<std::string>::iterator it=vec.begin() ; it!=vec.end();it++){
					std::cout<<*it<<std::endl;
				}
			vec.clear();
		}


	// example for mysql_query
	process.set_mysql_query("select * from weibo limit 2");
	process.Query_DB();
	std::list<std::list<std::string> > result=process.Get_result();
	process.display(result);

/*	std::set<std::string> stopwords;
	std::list<std::string> vec;
	std::list<std::list<std::string> > vecs;
	std::list<std::string> IDs;
	MakeStopSet(stopwords);
	std::string ID="2013-04-12 08:38:16";

		std::string fenci;
	double total;
	char mysql_q[60];

	time_t startT,endT;

	IDs=Get_MIDs("2013-04-12 08:38:16",4400);
		startT=time(NULL);
		for(std::list<std::string>::iterator it_ids=IDs.begin();it_ids!=IDs.end();it_ids++){
			ID=*it_ids;
			sprintf(mysql_q,"select text from weibo where mid=%s ",ID.c_str());
			vecs=mysql_query(mysql_q);
			std::cout<<ID<<std::endl;
			fenci=vecs[0][0];
			ICTspilt(fenci.c_str());
		//et_Stringlist(ID,stopwords,vec);




		//	for(std::list<std::string>::iterator it=vec.begin() ; it!=vec.end();it++){
			//		fenci.append(*it+"|");
		//		}
		//	insert_fenci(ID,fenci);

			vec.clear();
		 //   fenci.erase();
		}

	endT=time(NULL);
	total=difftime(endT,startT);
	std::cout<<"程序运行时间"<<total<<"   num"<<IDs.size()<<std::endl;
****************************************************************************************************************/

	getchar();
	return 1;
}

#endif
/**
 * 		分词函数
 */
 std::string Process::ICTspilt(const char * sinput){

  std::string result;

  //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
  ICTCLAS_SetPOSmap(2);

  unsigned int nPaLen=strlen(sinput); // 需要分词的长度
  char* sRst=0;   //用户自行分配空间，用于保存结果；
  sRst=(char *)malloc(nPaLen*6); //建议长度为字符串长度的倍。
  int nRstLen=0; //分词结果的长度

  nRstLen = ICTCLAS_ParagraphProcess(sinput,nPaLen,sRst,CODE_TYPE_UNKNOWN,0);  //字符串处理
  result=sRst;
  free(sRst);
  return result;
}


/*****************************************************************************************
 * 		去噪分割函数
 * 		首先是则正表达式
 * 		然后是中文分词
 * 		最后是停此表
 */
void Process::goodWordsinPieceArticle(std::list<std::string> &goodword){

  std::list<std::string> goodWordstemp;
  const std::string temp=Regex_Replace(rawtext);  //先正则表达式
  std::string  result=ICTspilt(temp.c_str());
  boost::split(goodWordstemp,result ,boost::is_any_of(" "));

  for(std::list<std::string>::iterator it=goodWordstemp.begin();it!=goodWordstemp.end();it++){
    std::string temp=*it;
    boost::trim(temp);
    if(!stopwordsSet.count(temp)&&!temp.empty()){
       goodword.push_back(temp);
	  }
	}
}

/***************************************************************************************************
 * 获得停词
 */
void Process::MakeStopSet(const char * path){
	std::ifstream ifile(path);
    if(!ifile){
    	std::cout<<"打开停词表失败"<<std::endl;
    	return ;
    }
	while(!ifile.eof()){
      std::string temp;
		//getline(ifile,temp);	
	  ifile>>temp;
	  boost::trim(temp);
	  stopwordsSet.insert(temp);
	}
}
/***************************************************************************************************
 * mysql查询函数
 *
 * 查询语句在  sql_query
 * 结果放在       result
 *
 */
 void Process::Query_DB(){
	 int res;
	 MYSQL_RES *res_ptr;
	 MYSQL_ROW sqlrow;
	 if(!result.empty())
	 result.clear();
	 mysql_init(&my_connection);     
	 if (mysql_real_connect(&my_connection, "localhost", "", "", "test", 0, NULL, 0)){

				if ( mysql_set_character_set( & my_connection, "utf8") ) {
					fprintf(stderr,"设置中文 %s \n",mysql_error(&my_connection));
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
								std::list<std::string> v;
								std::list< std::list<std::string> > temp;
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
}

/**************************************************************************
 * 显示list二维里面的内容
 */
void Process::display(std::list< std::list<std::string> > &msg)
{
	
	for(std::list< std::list<std::string> >::iterator its=msg.begin();its!=msg.end();its++){
		for(std::list<std::string>::iterator it=(*its).begin();it!=(*its).end();it++)
		{
			std::cout<<*it<<std::endl;
		}

	}



}

void Process::Get_MIDs(std::string starttimes, int seconds,std::list<std::string> &MIDs)
{
	char  endtime[20];
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
	Query_DB();
	for(std::list< std::list<std::string> >::iterator iter=result.begin();iter!=result.end();iter++){ //注意clear 是否清空到0
			MIDs.push_back((*iter).front());
	}
}


void Process::Get_Stringlist(std::string &mid,std::list<std::string> &vec){

	if(mid.empty()){
		printf("____________________Get_Stringlist funciton imput error_____________\n");
		return ;
	}

	sprintf(sql_query,"select text from weibo where mid=%s ",mid.c_str());
	Query_DB();
	rawtext=result.front().front();
	if(!rawtext.empty())
	goodWordsinPieceArticle(vec);
}



/**********************************************************
 * 		"2014-12-07 12:03:11" ---------------->struct tm
 * 		时间格式转换
 */
struct tm Process::tranformTime(std::string &starttime ){
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

/**
 * 窄字符转换成宽字符
 */
std::wstring Process::StringToWide(std::string &sToMatch){

	std::setlocale(LC_ALL,"zh_CN.UTF-8");

	int iWLen= mbstowcs( NULL, sToMatch.c_str(), sToMatch.length() );
	wchar_t *lpwsz= new wchar_t[iWLen+1];
	int i= mbstowcs( lpwsz, sToMatch.c_str(), sToMatch.length() );
	std::wstring wsToMatch(lpwsz);
	std::setlocale(LC_ALL,"");
	delete []lpwsz;
	return wsToMatch;
}
/**
 * 宽字符转换成窄字符
 */
std::string Process::WidetoString(std::wstring &wsm){

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

/**
 * 正则表达式去噪
 * input 需要去噪的句子
 * output 已经去砸的句子
 *  1 表情  如 [高兴]    								 regex：-----------------   \\[([^x00-xff]*)\\]
 *  2 @微博 名         @traa @傅fuuuu @记得我叫青蛙忘了我叫俞映庭:  regex  -------- /?/?\@([^ ]*)[:]|/?/?\@([^ ]*)[ ]   以空格和:结尾
 *  3 超链接    http									 regex -----------------[a-zA-z]+://[^s]*
 *  3 一些特殊的字符              ~@#￥%%……&……                   regex ----------------[()——_*$#]*|【动画】|〜
 */



std::string Process::Regex_Replace(std::string input){
	std::string output;
	std::wstring put;
	//去掉表情
	input.append(" ");
	boost::wregex reg(L"\\[([^x00-xff]*)\\]|/?/?\@([^ ]*)[:]|/?/?\@([^ ]*)[ ]|[a-zA-z]+://[^s]*", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg,std::wstring(L""));
	input=WidetoString(put);


	//去掉@人名转发     思路去输掉空格                  @非空格+空格
//	boost::wregex reg2(L"/?/?\@([^ ]*)[ ]", boost::regex::perl);


	//匹配非数字英文中文等特殊符号
	boost::wregex reg2(L"【动画】|[()——_*$#┮【】「┭∠」ʑ≦≧▽з؞]*|〜", boost::regex::perl);
	put=boost::regex_replace(StringToWide(input),reg2,std::wstring(L""));
	output=WidetoString(put);

//	boost::wregex reg3(L"[^0-9a-zA-Z\u4e00-\u9fa5 ]*|〜", boost::regex::perl);
//	put=boost::regex_replace(StringToWide(input),reg3,std::wstring(L""));
//	output=WidetoString(put);

	return output;
	
}


Process::Process(){
	  //init the ictcals
	  if(!ICTCLAS_Init())  {

	    printf("Init fails\n");
	  }
	  else{
        printf("Init ok\n");
	  }
	  //设置词性标注集(0 计算所二级标注集，1 计算所一级标注集，2 北大二级标注集，3 北大一级标注集)
	  ICTCLAS_SetPOSmap(2);

	  ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_GB);
	  ICTCLAS_SaveTheUsrDic();//保存用户词典

	  MakeStopSet("stopwords.txt");
}


std::list< std::list<std::string> >  Process::Get_result(void){
	return this->result;
}


void Process::set_mysql_query(const char * sql){

  if(sql==NULL||strlen(sql)>=100) {
	  printf("____________________set_mysql_query funciton imput error_____________\n");
  }
  strcpy(sql_query,sql);
}




