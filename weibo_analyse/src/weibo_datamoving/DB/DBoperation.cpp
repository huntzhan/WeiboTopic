#include "DBoperation.h"



void DBoperation::DBConnect(){
	//初始化插入
	mysql_init(&my_connection);
	if (mysql_real_connect(&my_connection, mysql_addr.c_str(), mysql_usr.c_str(),mysql_pwd.c_str(), mysql_database.c_str(), 0, NULL, 0)){
			if ( mysql_set_character_set( & my_connection, "utf8") ) {
			fprintf(stderr,"设置中文失败 %s \n",mysql_error(&my_connection));
			}
		}
	 else {
			fprintf(stderr, "Insert_Connection failed\n");
		    if (mysql_errno(&my_connection))
				{
					fprintf(stderr, "Insert_Connection error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		}
	 }

}


 void DBoperation::DB_query(const char * sql_query,std::list<std::list<std::string> > &result){
	int res;
	MYSQL_RES *res_ptr;
	MYSQL_ROW sqlrow;
	std::string m_str;
	if(!result.empty())
	result.clear();
	res=mysql_query(&my_connection,sql_query);
	if (res) {
				fprintf(stderr, "SELECT error: %s\n", mysql_error(&my_connection));
	} else {
			res_ptr = mysql_use_result(&my_connection);   //这个函数有问题
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
			    		   sqlrow[i]=0;
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


}



bool DBoperation::DB_insertData(INSERT_DATA &insert_data){
	int res;

	char sql_query[2048];
	char goodtext[1024];
	const char *row=insert_data.text.c_str();
	mysql_escape_string(goodtext,row,strlen(row));
	sprintf(sql_query,"insert into %s values('%s','%s','%s')",table_name.c_str(),insert_data.mid.c_str(),goodtext ,insert_data.spilt.c_str());
//	std::cout<<sql_query<<std::endl;

	res=mysql_query(&my_connection,sql_query);
	if (res) {
		fprintf(stderr, "insert error: %s\n", mysql_error(&my_connection));
	}
	return false;
}

void DBoperation::CreateTable(){
	int res;
	char sql_query[1024];
	sprintf(sql_query,"	CREATE TABLE `%s` (`mid` varchar(50) NOT NULL, `text` varchar(250) DEFAULT NULL, `spilt` varchar(1000) DEFAULT NULL,  PRIMARY KEY (`mid`)) ENGINE=InnoDB DEFAULT CHARSET=utf8",table_name.c_str());
	res=mysql_query(&my_connection,sql_query);
	if (res) {
			fprintf(stderr, "insert error: %s\n", mysql_error(&my_connection));
	}
}


void DBoperation::SetTableName(const std::string table_name){
	this->table_name=table_name;
}


/**
 * 获得标的行数用于游标操作
 */
long  DBoperation::Getcount(){
	std::list<std::list<std::string> > result;
	char sql_query[100];
	sprintf(sql_query,"select count(*) from %s ",table_name.c_str());
	std::cout<<sql_query<<std::endl;
	DB_query(sql_query,result);
	long count=atol(result.front().front().c_str());
	return  count;
}

void DBoperation::GetText(long startline,long length,std::list<std::list<std::string> > &result){
	char sql_query[200];
	sprintf(sql_query,"select mid,content from %s limit %ld ,%ld ",table_name.c_str(),startline,length);
	DB_query(sql_query,result);
}

void DBoperation::GetTables(std::list<std::string> &tables){
	std::list<std::list<std::string> > result;
	char sql_query[200];
	DB_query("show tables",result);

	std::list<std::list<std::string> >::iterator it_list=result.begin();
	std::list<std::list<std::string> >::iterator end_list=result.end();

		for(;it_list!=end_list;it_list++){
			std::string table=it_list->back();
			if(table[0]=='M')
			{
			std::cout<<it_list->back()<<std::endl;
				tables.push_back(it_list->back());
			}
		}
}
















