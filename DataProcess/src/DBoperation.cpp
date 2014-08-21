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
void DBoperation::DBTableInit(int weibo_size,int OneTimeReadWeiboNum,int tableIndex,std::list<std::string>table){
	this->weibo_size=weibo_size;
	this->OneTimeReadWeiboNum=OneTimeReadWeiboNum;
	this->tableIndex=tableIndex;
	std::copy(table.begin(), table.end(), std::back_inserter(this->table));
}
void DBoperation::DBinit(std::string  database,ConnPool *connpool){
	m_connpool=connpool;ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
	con = m_connpool->GetConnection();
	state = con->createStatement();
	state->execute(database);

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
	} catch (sql::SQLException&e) {
		perror(e.what());
	}
	long count=atol(str.c_str());
	return  count;
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
			//		std::cout<<word.word<<"|"<<word.proper<<std::endl;
		}
		 result.push_back(originalword);
    }
	resultsql->close();
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
void DBoperation::ShowTable(std::list<std::string> &tables){
	ResultSet *result;
	try {
		result = state->executeQuery("show tables");
			while (result->next()) {
				tables.push_back(result->getString(1));
		}
	} catch (sql::SQLException&e) {
		perror(e.what());
	}
}


DBoperation::~DBoperation(){
	DBclose();
}

void DBoperation::InsertData(Topic &onetopic, int flag) {
  char table_name[512];
  std::string topictablename = "Topic";
  int newestID = 0;

  //数据插入，如果是第一次插入就要先将topic下的信息先插入数据库，然后再建表
  try {
    if (flag == 0) {
      std::string mytablename = "OneDayTopic";
      std::string topicwords;
      int weibonumbers = onetopic.topic_message_num;

      con->setAutoCommit(false);
      char sql_query[1024];
      list<TopicWord>::iterator it = onetopic.m_stopic.begin();
      for (; it != onetopic.m_stopic.end(); ++it) {
        topicwords += it->m_sword + " ";
      }

      sprintf(sql_query, "insert into %s values('','%s','','%d')",
          mytablename.c_str(), topicwords.c_str(), weibonumbers); //(topicid,topicwords,mainidea,weibonumber)

      state->executeUpdate(sql_query);
      con->commit();
      con->setAutoCommit(true);
      newestID = GetNewserID();

      sprintf(table_name, "%s%d", topictablename.c_str(), newestID);

      CreateTable(table_name);
    }

    //第二次插入只要查询Topic的ID，然后插入数据
    newestID = GetNewserID();
    sprintf(table_name, "%s%d", topictablename.c_str(), newestID);
    InsertTopicWeiboIdToDatabase(onetopic, table_name);
    onetopic.topic_weibo.clear();
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
int DBoperation::GetNewserID(){
	ResultSet *result;
	int res;
	char sql_query[1024];
	sprintf(sql_query, "select max(topicid) from OneDayTopic");
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
		std::list<Weibo>::iterator it = onetopic.topic_weibo.begin();
		for(;it!= onetopic.topic_weibo.end();++it) {

			const char* text=it->text.c_str();
			mysql_escape_string(goodtext,text,strlen(text));

			sprintf(sql_query,"insert into %s values('%s','','%s','%s','%s','','','','','')",
					mytablename.c_str(),it->mid.c_str(),goodtext,it->spilt.c_str(),it->belongtable.c_str());
			state->executeUpdate(sql_query);
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
		if (this->tableIndex == 2){//this->table.size()-1
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


