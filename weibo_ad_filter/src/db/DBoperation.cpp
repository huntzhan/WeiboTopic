#include "DBoperation.h"


DBoperation::DBoperation(std::string mysql_addr, 
            std::string mysql_usr,
            std::string mysql_pwd, 
            std::string mysql_database) {
  this->mysql_addr = mysql_addr;
  this->mysql_usr = mysql_usr;
  this->mysql_pwd = mysql_pwd;
  this->mysql_database = mysql_database;
}

void DBoperation::DBConnect(){
  //初始化插入
  mysql_init(&my_connection);
  if (mysql_real_connect(&my_connection, 
                         mysql_addr.c_str(), 
                         mysql_usr.c_str(),
                         mysql_pwd.c_str(), 
                         mysql_database.c_str(), 
                         0, 
                         NULL, 
                         0)){
      if ( mysql_set_character_set( & my_connection, "utf8") ) {
        fprintf(stderr,"设置中文失败 %s \n",mysql_error(&my_connection));
      }
    }
   else {
      fprintf(stderr, "Insert_Connection failed\n");
      if (mysql_errno(&my_connection)){
          fprintf(stderr, "Insert_Connection error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
      }
   }

}


 void DBoperation::DB_query(const char * sql_query,
                            std::list<std::list<std::string> > &result){
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
//  std::cout<<sql_query<<std::endl;

  res=mysql_query(&my_connection,sql_query);
  if (res) {
    fprintf(stderr, "insert error: %s\n", mysql_error(&my_connection));
  }
  return false;
}

void DBoperation::CreateTable(){
  int res;
  char sql_query[1024];
  sprintf(sql_query,"  CREATE TABLE `%s` (`mid` varchar(50) NOT NULL, `text` varchar(250) DEFAULT NULL, `spilt` varchar(1000) DEFAULT NULL,  PRIMARY KEY (`mid`)) ENGINE=InnoDB DEFAULT CHARSET=utf8",table_name.c_str());
  res=mysql_query(&my_connection,sql_query);
  if (res) {
      fprintf(stderr, "insert error: %s\n", mysql_error(&my_connection));
  }
}


void DBoperation::SetTableName(const std::string table_name){
  this->table_name=table_name;
  this->table_user_name=table_name;
  this->table_user_name.replace(0,9,"WeiboUser");
  this->table_user_to_blog=table_name;
  this->table_user_to_blog.replace(0,9,"UserToBlog");
  std::cout<< this->table_user_to_blog<<std::endl;
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
/**
 * 获得文本内容返回一个二维的数组
 */
void DBoperation::GetText(long startline,long length,std::list<std::list<std::string> > &result){
  char sql_query[1024];
 // sprintf(sql_query,"select mid,content from %s limit %ld ,%ld ",table_name.c_str(),startline,length);
  sprintf(sql_query,"select %s.mid ,%s.uid, created_time, content,favorites,comments,forwards,source ,\
      followees,fans,posts,favourites_count,created_at,verified , bi_followers_count,sex from %s ,%s, %s where %s.mid=%s.mid and %s.uid=%s.uid limit %ld,%ld ",
      table_name.c_str(),table_user_name.c_str(),
      table_name.c_str(),table_user_name.c_str(),table_user_to_blog.c_str(),
      table_name.c_str(),table_user_to_blog.c_str(),
      table_user_name.c_str(),table_user_to_blog.c_str(),
      startline,length
      );
  // std::cout<< sql_query<<std::endl;
  DB_query(sql_query,result);
}

void DBoperation::GetTables(std::list<std::string> &tables){
  std::list<std::list<std::string> > result;
  DB_query("show tables",result);

  std::list<std::list<std::string> >::iterator it_list=result.begin();
  std::list<std::list<std::string> >::iterator end_list=result.end();

    for(;it_list!=end_list;it_list++){
      std::string table=it_list->back();
      if(table[0]=='M')
      {
        tables.push_back(it_list->back());
      }
    }
}


void DBoperation::GetWeiBos(long startline,long length,std::list<Blog> &weibos){
  std::list<std::list<std::string> > result;
  GetText(startline,length,result);
  std::list<std::list<std::string> >::iterator it_first=result.begin();
  std::list<std::list<std::string> >::iterator end_first=result.end();
  for(;it_first!=end_first;it_first++){
     list<string>::iterator it_second=it_first->begin();
     list<string>::iterator end_second=it_first->end();
     Blog blog;
     blog.m_mid=*it_second;
     it_second++;
     blog.u_uid=*it_second;
     it_second++;
     blog.m_created_time=*it_second;
     it_second++;
     blog.m_content=*it_second;
     it_second++;
     blog.m_favorites=atol((*it_second).c_str());
     it_second++;
     blog.m_comments=atol((*it_second).c_str());
     it_second++;
     blog.m_forwards=atol((*it_second).c_str());
     it_second++;
     blog.m_source=*it_second;
     it_second++;
     blog.u_followees=atol((*it_second).c_str());
     it_second++;
     blog.u_fans=atol((*it_second).c_str());
     it_second++;
     blog.u_posts=atol((*it_second).c_str());
     it_second++;
     blog.u_favourites_count=atol((*it_second).c_str());
     it_second++;
     blog.u_created_at=*it_second;

     it_second++;
     blog.u_vierfied=atol((*it_second).c_str());
     it_second++;
     blog.u_bi_followers_count=atol((*it_second).c_str());
     it_second++;
     blog.sex=*it_second;
     weibos.push_back(blog);
  }
}















