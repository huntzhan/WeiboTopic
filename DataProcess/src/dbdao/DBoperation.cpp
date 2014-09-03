#include "DBoperation.h"

/**
 * 初始化化数据库
 */
std::vector<std::string> split(std::string str){
	std::vector<std::string>result;
	std::string mid=str.substr(0,16);
	std::string table=str.substr(16,str.size());
	result.push_back(mid);
	result.push_back(table);
	return result;
}


void DBoperation::SelectOnetableByBatch(std::string table, std::list<std::string> &weibolist, std::list<Weibo> &weibocontentlist){

  std::string weibo_mid_set;
  int size=weibolist.size();
  std::list<std::string>::iterator it =weibolist.begin();
  for(;it != weibolist.end(); ++it){
    if(--size<=0)break;
    weibo_mid_set+=*it+",";
  }

  weibo_mid_set+=*it;
//  std::cout<<weibo_mid_set<<std::endl;
  const char* weibo_mid_set_cstr=weibo_mid_set.c_str();
  int len = strlen(weibo_mid_set_cstr);
  char SQL[2*len+128];
  sprintf(SQL,"select mid,text,spilt from  %s where mid in (%s)",table.c_str(),weibo_mid_set_cstr);
//  std::cout<<"run after SQL"<<std::endl;
  ResultSet * result;
  std::string split;
  std::string text;
  try{
    result=state->executeQuery(SQL);
//    std::cout<<"run after execute SQL"<<std::endl;
    while(result->next()){
      text = result->getString(2);
      split = result->getString(3);
      Weibo weibo;
      weibo.mid=result->getString(1);
      weibo.SetWeiboContentWords(split, text);
      weibocontentlist.push_back(weibo);
//      std::cout<<"run in get data"<<std::endl;
    }
    result->close();
  }catch (sql::SQLException &e) {
    perror(e.what());
  }

}
std::string DBoperation::Gentime(){
  time_t t;
  t = time(0);
  char now[64];
  struct tm *ttime;
  //将获得的时间进行各种转换的结构体
  ttime = localtime(&t);
  strftime(now, 64, "%Y%m%d", ttime);
  string strtime(now);
  return strtime;
}
std::string DBoperation::GenDayAndTime(){
  time_t t;
  t = time(0);
  char now[64];
  struct tm *ttime;

  ttime = localtime(&t);
  strftime(now, 64, "%Y%m%d_%H", ttime);
  string strtime(now);
  return strtime;
}
std::string GenDayAndTimestamp(std::string format){
  time_t t;
  t = time(0);
  char now[64];
  struct tm *ttime;

  ttime = localtime(&t);
  strftime(now, 64, format.c_str(), ttime);
  string strtime(now);
  return strtime;
}
time_t str2time(std::string strtime){
  tm tm_;
  time_t t_;
  char buf[128]= {0};

  strcpy(buf, strtime.c_str());
  strptime(buf, "%Y-%m-%d %H:%M:%S", &tm_); //将字符串转换为tm时间
  tm_.tm_isdst = -1;
  t_  = mktime(&tm_); //将tm时间转换为秒时间
  return t_;
}
std::string formatTimeAtHour(){
  char mytime[64];
  sprintf(mytime,"%d",str2time(GenDayAndTimestamp("%Y-%m-%d %H:00:00")));
  std::cout<<mytime<<std::endl;
  std::string strtime(mytime);
  return strtime;
}
std::string formatTimeAtDay(){
  char mytime[64];
  sprintf(mytime,"%d",str2time(GenDayAndTimestamp("%Y-%m-%d 00:00:00")));
  std::cout<<mytime<<std::endl;
  std::string strtime(mytime);
  return strtime;
}
void DBoperation::DBTableInit(int weibo_size,int OneTimeReadWeiboNum,int tableIndex,std::list<std::string>table){
	this->weibo_size=weibo_size;
	this->OneTimeReadWeiboNum=OneTimeReadWeiboNum;
	this->tableIndex=tableIndex;
	std::copy(table.begin(), table.end(), std::back_inserter(this->table));
}
void DBoperation::DBinit(std::string  database,std::string topic_table_name, ConnPool *connpool){
	m_connpool=connpool;
//	ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
	con = m_connpool->GetConnection();
	state = con->createStatement();
	state->execute("use "+database);
//	this->topic_table_name=this->GenDayAndTime();
	this->topic_table_name=topic_table_name;
	this->database_name  = database;

}
/*@description：
 *  参照上面的方法建立新的连接，同时创建执行sql语句的句柄。
 *@input:
 *  需要查询的话题段
 */

void QueryTopicWeibo(int start,int end, std::vector<Topic> &topiclist){

}
void DBoperation::DBclose(){
	m_connpool->ReleaseConnection(con);
}

void DBoperation::SetTableName(const std::string table_name){
	this->table_name=table_name;
}
/**
 * 获得标的总行数
 */
long  DBoperation::GetTablecount(){
	ResultSet *result;
	string str;
	char sql_query[100];
	sprintf(sql_query,"select count(*) from %s ",table_name.c_str());
	try {
		result = state->executeQuery(sql_query);
		while (result->next()) {
				str = result->getString(1);
		}
	} catch (sql::SQLException &e) {
		perror(e.what());
	}
	long count=atol(str.c_str());
	return  count;
	result->close();
}
/**
 * 从数据库充读取已经分好的词，startline就是开始的行 length就是要读取的行数
 */
void DBoperation::GetMidandText(long startline,long length,std::list<OneWeibo> &result){
	ResultSet *resultsql;
	char sql_query[200];
	sprintf(sql_query,"select mid,spilt from %s limit %ld ,%ld ",table_name.c_str(),startline,length);
	resultsql = state->executeQuery(sql_query);
	while (resultsql->next()) {
		OneWeibo originalword;
		originalword.MID = resultsql->getString(1);
		std::string spilt = resultsql->getString(2);
		std::vector<std::string> goodWordstemp;
		boost::split(goodWordstemp, spilt, boost::is_any_of(" ")); //分割

		std::vector<std::string>::iterator it_word = goodWordstemp.begin();
		std::vector<std::string>::iterator end_word = goodWordstemp.end();
		for (int i = 0; i < goodWordstemp.size() - 1; i++) {
			Word word;
			word.word = goodWordstemp[i];
			i++;
			word.proper = goodWordstemp[i];

			originalword.words.push_back(word);
		}
		 result.push_back(originalword);
    }
	resultsql->close();
}
/**
 * 从数据库充读取已经分好的词，startline就是开始的行 length就是要读取的行数
 */
void DBoperation::GetMidandTextAndSplit(long startline,long length,std::list<OneWeibo> &result){
  ResultSet *resultsql;
  char sql_query[200];
  std::vector<std::string>regexresult;
  std::string text;
  sprintf(sql_query,"select mid,text,spilt from %s limit %ld ,%ld ",table_name.c_str(),startline,length);
  resultsql = state->executeQuery(sql_query);
  while (resultsql->next()) {
    OneWeibo originalword;
    originalword.MID = resultsql->getString(1);
    text=resultsql->getString(2);
    std::string spilt = resultsql->getString(3);

    regexresult=RegexTagWord(text);


    std::vector<std::string> goodWordstemp;
    boost::split(goodWordstemp, spilt, boost::is_any_of(" ")); //分割

    std::vector<std::string>::iterator it_word = goodWordstemp.begin();
    std::vector<std::string>::iterator end_word = goodWordstemp.end();
    for (int i = 0; i < goodWordstemp.size() - 1; i++) {
      Word word;
      word.word = goodWordstemp[i];
      i++;
      word.proper = goodWordstemp[i];

      //判断该词否在tag中
      if(iswordinregexresult(regexresult, word.word)){
        word.is_tag_word=true;
      }
      else{
        word.is_tag_word=false;
      }

      originalword.words.push_back(word);
    }
     result.push_back(originalword);
    }
  resultsql->close();
}
bool DBoperation::iswordinregexresult(std::vector<std::string>&regexresult, std::string word){
  std::vector<std::string>::iterator it= regexresult.begin();
  std::string str;

  int index=-1;
  for(;it != regexresult.end();++it){
    str=*it;
    index = str.find(word);

    if(0<=index&&index<str.length()){
      return true;
    }
  }
  return false;
}
std::vector<std::string> DBoperation::RegexTagWord(std::string &weibo_origin_text){
  std::vector<std::string>resVec;
  boost::regex expr("#[\\S]+#");
  boost::smatch what;
  std::string::const_iterator start = weibo_origin_text.begin();
  std::string::const_iterator end = weibo_origin_text.end();
  while(boost::regex_search(start,end, what, expr)) {
    resVec.push_back(what[0]);
//    std::cout<<what[0]<<std::endl;
    start = what[0].second;
  }
  return resVec;
}
/**
 *输入一个MID返回对应的微博
 *
 */
void DBoperation::GetOneWeiBo(std::string table_name1,std::string MID,Weibo &oneweibo){
	ResultSet *result;
	char sql_query[200];
	sprintf(sql_query,"select text,spilt from %s where mid=%s ",table_name1.c_str(),MID.c_str());
	try {
		result = state->executeQuery(sql_query);
		while (result->next()) {
				std::string text= result->getString(1);
				std::string spilt=result->getString(2);
				oneweibo.SetWeiboContentWords(spilt,text);
				oneweibo.mid=MID;
		}
	} catch (sql::SQLException&e) {
		perror(e.what());
	}

}
/**
 * 查询数据库下面有多少张表
 */
void DBoperation::ShowTable(std::list<std::string> &tables,int tablenum){
	ResultSet *result;
	int count=0;
	try {
		result = state->executeQuery("show tables");
			while (result->next()) {
			  if(++count>=tablenum)break;
				tables.push_back(result->getString(1));
		}
	} catch (sql::SQLException&e) {
		perror(e.what());
	}
}


DBoperation::~DBoperation(){
	DBclose();
}
void DBoperation::CreateOneDayTopicTable(std::string mytablename) {

  std::string  statement= "CREATE TABLE `%s` "
      "(`topicid` int(20) NOT NULL AUTO_INCREMENT,"
      "`topicwords` varchar(1000) DEFAULT NULL,"
      "`mainidea` varchar(1000) DEFAULT NULL,"
      "`weibonumber` int(11) default null,  "
      "`isPolitic` int(11) default null,"
      "PRIMARY KEY (`topicid`) ) "
      "ENGINE=InnoDB DEFAULT CHARSET=utf8;";

  char sql_query[1024];
  sprintf(sql_query,statement.c_str(),mytablename.c_str());
  std::cout<<"建表语句"<<sql_query<<std::endl;
  try {
    state->execute(sql_query);
    std::cout<<"数据库创建成果"<<std::endl;
  } catch (sql::SQLException&e) {
    perror(e.what());
  }

}
bool DBoperation::QueryIfTableExist(std::string tablename){
  char sql_query[1024];
  ResultSet *result;
  std::string statement="select table_name from information_schema.tables where table_name='%s'";

  sprintf(sql_query,statement.c_str(),tablename.c_str());
  std::cout<<"查表语句"<<sql_query<<std::endl;
  try{
    result=state->executeQuery(sql_query);
    if(result->next()){
      std::cout<<"数据库OneDayTopic系列已存在"<<std::endl;
        return true;
    }else{
      return false;
    }
  }catch(sql::SQLException &e){
    perror(e.what());
  }
}
void DBoperation::QueryIfOneDayTableExistAndCreate(){

  std::string onedaytopic_tablename="OneDayTopic_"+topic_table_name;
  std::string query_table_name="OneDayTopic_"+topic_table_name;
  //这里用指定的数据库名字，可是还是不可以
//  std::string query_table_name=this->database_name+".OneDayTopic_"+Gentime();
  std::cout<<"tablename: "<<query_table_name<<std::endl;
  if(QueryIfTableExist(query_table_name))
    return;
  else{
    CreateOneDayTopicTable(onedaytopic_tablename);
  }
}
void DBoperation::InsertData(Topic &onetopic, int flag, ofstream &outfile) {
  char table_name[512];
  std::string topictablename = "Topic_"+this->topic_table_name+"_";
  int newestID = 0;

  //数据插入，如果是第一次插入就要先将topic下的信息先插入数据库，然后再建表
  try {
//    std::string OneDayTopicTablename = "OneDayTopic_"+Gentime();
    std::string OneDayTopicTablename = "OneDayTopic_"+topic_table_name;
    if (flag == 0) {
      //为进程加锁
//      pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
//      pthread_mutex_init(&job_queue_mutex, NULL);
//      pthread_mutex_lock (&job_queue_mutex);

//      std::string mytablename = "OneDayTopic_"+Gentime();
      std::string topicwords;
      int weibonumbers = onetopic.topic_message_num;
      std::string main_idea;
      std::list<subword>::iterator mainidea_it = onetopic.GetSubWordList()->begin();
      for(;mainidea_it!=onetopic.GetSubWordList()->end();++mainidea_it){
        main_idea+=mainidea_it->word+" ";
      }
//      con->setAutoCommit(false);
      char sql_query[1024];
      list<TopicWord>::iterator it = onetopic.m_stopic.begin();
      for (; it != onetopic.m_stopic.end(); ++it) {
        topicwords += it->m_sword + " ";
      }
      sprintf(sql_query, "insert into %s values('','%s','%s','%d','%d')",
          OneDayTopicTablename.c_str(), topicwords.c_str(), main_idea.c_str(),weibonumbers,onetopic.isPolitic); //(topicid,topicwords,mainidea,weibonumber)

      state->executeUpdate(sql_query);
      con->commit();
//      con->setAutoCommit(true);
      newestID = GetNewserID(OneDayTopicTablename);

      sprintf(table_name, "%s%d", topictablename.c_str(), newestID);

      CreateTable(table_name);
      std::string oneline(table_name);
      outfile<<this->database_name+"."+oneline+","; 
      //解锁
//      pthread_mutex_unlock (&job_queue_mutex);
    }

    //第二次插入只要查询Topic的ID，然后插入数据
    newestID = GetNewserID(OneDayTopicTablename);
    sprintf(table_name, "%s%d", topictablename.c_str(), newestID);
    InsertTopicWeiboIdToDatabase(onetopic, table_name);
//    onetopic.topic_weibo.clear();
//    std::cout<<"onetopic.topic_weibo size: "<<onetopic.topic_weibo.size()<<std::endl;
  } catch (sql::SQLException&e) {
    perror(e.what());
  }
  std::cout << "插入一个话题" << std::endl;
}
void DBoperation::CreateTable(std::string mytablename) {

	std::string  statement= "CREATE TABLE `%s` "
			"(`mid` varchar(50) NOT NULL,"
			"`created_time` varchar(50) DEFAULT NULL,"
			"`content` varchar(255) DEFAULT NULL,"
			"`split` varchar(350) default null,"
			"`belongtable` varchar(50) default null,"
			"`favorites` int(11) DEFAULT NULL,"
			"`comments` int(11) DEFAULT NULL,"
			"`forwards` int(11) DEFAULT NULL,"
			"`source` varchar(50) DEFAULT NULL,"
			"`subTopic` varchar(50) DEFAULT NULL,"
			"PRIMARY KEY (`mid`)) "
			"ENGINE=InnoDB DEFAULT CHARSET=utf8;";

	char sql_query[1024];
	sprintf(sql_query,statement.c_str(),mytablename.c_str());
//	std::cout<<sql_query<<std::endl;
	try {
		state->execute(sql_query);

	} catch (sql::SQLException&e) {
		perror(e.what());
	}

}
int DBoperation::GetNewserID(std::string OneDayTopic_name){
	ResultSet *result;
	int res;
	char sql_query[1024];
	sprintf(sql_query, "select max(topicid) from %s",OneDayTopic_name.c_str());
	try {
		result=state->executeQuery(sql_query);
		while (result->next()) {
			res=result->getInt(1);

		}
	} catch (sql::SQLException&e) {
		perror(e.what());
	}
	return res;
}


void DBoperation::InsertTopicWeiboIdToDatabase(Topic &onetopic,std::string mytablename){
	try {
		con->setAutoCommit(false);
		char sql_query[2048];
		char goodtext[1024];
		string midandtable;
		std::string mid;
//		std::cout<<"onetopic.topic_weibo size: "<<onetopic.topic_weibo.size()<<std::endl;
		std::list<Weibo>::iterator it = onetopic.topic_weibo.begin();
		for(;it!= onetopic.topic_weibo.end();++it) {

			const char* text=it->text.c_str();
			mysql_escape_string(goodtext,text,strlen(text));

			sprintf(sql_query,"insert into %s values('%s','','%s','%s','%s','','','','','')",
					mytablename.c_str(),it->mid.c_str(),goodtext,it->spilt.c_str(),it->belongtable.c_str());
			state->executeUpdate(sql_query);
//			std::cout<<"update 数据"<<std::endl;
		}
		con->commit();
	} catch (sql::SQLException&e) {
		perror(e.what());
	}
}

/*
 * 同一个数据库游标的增长
 */
void DBoperation::ChangeOneTableCount(int & count){

	count += this->OneTimeReadWeiboNum;
	if (count >= this->weibo_size) {
		count = this->weibo_size-1;
	}

}
/*
 * 根据已有的表切换数据表，返回false表示已经便利完所有表
 */
bool DBoperation::ChangeToNextTable(int & count){

	if (count >= this->weibo_size-1) {
		this->tableIndex += 1;

		if (this->tableIndex == this->table.size()){//this->table.size()
			std::cout<<"扫描完一遍数据库"<<std::endl;
			return false;
		}

		std::string tablename=this->table[this->tableIndex];
		this->SetTableName(tablename);
		count = 0;
		this->weibo_size=(int)this->GetTablecount();
		std::cout<<"切换数据库,数据库微博数："<<this->weibo_size<<std::endl;
	}
	return true;
}


void DBoperation::SetTableIndexToZero(){
	this->tableIndex=0;
	std::string tablename=this->table[this->tableIndex];
	this->SetTableName(tablename);
	this->weibo_size=(int)this->GetTablecount();
}


std::wstring StringToWide(std::string &sToMatch) {
  int iWLen = 1024;
  wchar_t lpwsz[iWLen];
  mbstowcs(lpwsz, sToMatch.c_str(), iWLen - 2);
  lpwsz[iWLen - 2] = '\0';
  std::wstring wsToMatch(lpwsz);
  return wsToMatch;
}
/*
 * 宽字符转换成窄字符
 */
std::string WidetoString(std::wstring &wsm) {
  std::string sToMatch;
  int iLen = 1024;
  char lpsz[iLen];
  wcstombs(lpsz, wsm.c_str(), iLen - 2); // 转换。（没有结束符）
  lpsz[iLen - 2] = '\0';
  sToMatch = lpsz;
  return sToMatch;
}

std::vector<std::string> DBoperation::BoostMatchWord(std::string str){
  std::vector<std::string>resVec;
  boost::regex expr("[^\\s+]+");
  boost::smatch what;
  std::string::const_iterator start = str.begin();
  std::string::const_iterator end = str.end();
  while(boost::regex_search(start,end, what, expr)) {
    resVec.push_back(what[0]);
    start = what[0].second;
  }
  return resVec;
}

std::vector<std::string> DBoperation::stringSplitToVector(std::string  &str,int splitLen){
  std::setlocale(LC_ALL, "zh_CN.UTF-8");
  std::wstring  wstr=StringToWide(str);

  std::wstring temp ;
  std::wstring tempres;
  std::string restemp;
  for(int i=0;i<wstr.length();i+=splitLen){
    temp=wstr.substr(i,splitLen);
    tempres+=temp+L" ";
  }
  restemp=WidetoString(tempres);

  std::vector<std::string> goodWordstemp =this->BoostMatchWord(restemp);
//  boost::split(goodWordstemp, restemp, boost::is_any_of("   ")); //分割
//  std::cout<<restemp<<std::endl;
  return goodWordstemp;
}
void DBoperation::DropTable(std::string table_prefix){
  char sql_query[1024];
  std::string statement = "select concat('drop table ',table_name,';') from information_schema.tables "
      "where table_name like '%s'";
  ResultSet * result;
  sprintf(sql_query,statement.c_str(),table_prefix.c_str());
  try{
    result = state->executeQuery(sql_query);
    while(result->next()){
      std::string drop_statement = result->getString(1);
      std::cout<<drop_statement<<std::endl;
//      state->execute(drop_statement.c_str());
    }
  }catch(sql::SQLException &e){
    perror(e.what());
  }
}

/***********************************************************************************************************
 *
 *  write by jinfa
 */

void DBoperation::GetTagText(std::list<Coverage::WEIBO > &result,std::string content){
	ResultSet *resultsql;
	char sql_query[200];
	sprintf(sql_query,"select mid,%s from %s where %s REGEXP '[#＃][^＃#]*[#＃]' ",content.c_str(),table_name.c_str(),content.c_str());
	resultsql = state->executeQuery(sql_query);
	while (resultsql->next()) {
		Coverage::WEIBO weibo;
		weibo.mid=resultsql->getString(1);
		weibo.content=resultsql->getString(2);
		 result.push_back(weibo);
    }
	resultsql->close();
}

void DBoperation::getTopicMainIdea(std::vector<pair<int, string> > &result) {
	ResultSet *resultsql;
	char sql_query[200];
	sprintf(sql_query, "select topicid,mainidea from %s ",
			table_name.c_str());
	resultsql = state->executeQuery(sql_query);
	while (resultsql->next()) {
		int id = resultsql->getInt(1);
		string MainIdea = resultsql->getString(2);
		result.push_back(make_pair(id, MainIdea));
	}
	resultsql->close();
}
