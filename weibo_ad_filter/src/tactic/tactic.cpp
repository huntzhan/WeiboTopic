/**
 * @file    tactic.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 06:50:44 PM
 * @version 1.0
 * 
 */

#include "tactic.h"
/**
 *  @brief IsSpam examine the user based on their number of fans, followees, bi-follow, and posts
 *
 *  @param
 *  @return
 */
//static 变量
std::vector<std::set<std::string> > TopicTcatic::badTopic;
std::set<std::string> UserTactic::baduser;
bool ZombieTactic::IsSpam(const Blog &b) {
  if (b.u_vierfied)
    return false;
  if (b.u_fans <= 25 || 
      b.u_followees >= 1000 || 
      b.u_bi_followers_count*1.0 / b.u_followees < 0.2)
    return true;
  return false;
}



/**
 *  @brief read the badtopic.txt
 *         读取不好的主题词，如广告，天气等
 *  @param
 *  @return
 */
void TopicTcatic::GetBadTopic(std::vector<std::set<std::string> > &input){
  std::ifstream infile("badtopic.txt");
  std::string result;
  if(!infile){
	  std::cout<<"can not find thebadtopic.txt "<<std::endl;
	  return ;
  }
  while(!infile.eof()){
	  std::string::size_type pos=0;
	  ///获得一行的数据，就是一串的主题词
	  getline(infile,result);
	  result+=" ";
	  unsigned int size=result.length();
	  std::set<std::string> set;
	  ///以空格进行分割
	  for(unsigned int i=0 ;i<size;i++){
		  pos=result.find(" ",i);
		  if(pos<size){
			 std::string s=result.substr(i,pos-i);
			 set.insert(s);
			 std::cout<<s<<std::endl;
			 i=pos;
		  }
	  }
	  input.push_back(set);
  }
}

TopicTcatic::TopicTcatic(){
	if(badTopic.size()<1)
	GetBadTopic(badTopic);
	match=2;
}
/**
 *  @brief 判断该微博是否属于不好的主题词
 *  包含主题词数目大于3 match这个参数
 *  @param
 *  @return
 */
bool TopicTcatic::IsSpam(const Blog &b) {
  std::vector<std::string> goodword;
  Parser parser;
  ///分词 把微博内容分词
  parser.LexicalAnalysisWord(b.m_content,goodword);
  //迭代badtopic
  std::vector<std::set<std::string> >::iterator it_badtopic=badTopic.begin();
  std::vector<std::set<std::string> >::iterator end_badtopic=badTopic.end();
  for(;it_badtopic!=end_badtopic;it_badtopic++){
	  //迭代微博分词
	  int count=0;
	  std::vector<std::string>::iterator it_goodword=goodword.begin();
	  std::vector<std::string>::iterator end_goodword=goodword.end();
	  for(;it_goodword!=end_goodword;it_goodword++){
		  ///判断该微博是否属于该坏的话题
          if(it_badtopic->count(*it_goodword)){
        	  count++;
        	  if(count>match){
        		 // std::cout<<b.m_content<<std::endl;
        		  return true;
        	  }
          }
       }
  }
  return false;
}

/**
 *  @brief 获得用户黑名单
 *
 *  @param
 *  @return
 */
void UserTactic::GetBadUser(std::set<std::string> &input){
  std::ifstream infile("baduser.txt");
  std::string result;
  if(!infile){
     std::cout<<"can not find baduser.txt "<<std::endl;
     return ;
  }
  while(!infile.eof()){
    infile>>result;
    input.insert(result);
  }
}

UserTactic::UserTactic(){
  if(baduser.size()<1)
    GetBadUser(baduser);
}


bool UserTactic::IsSpam(const Blog &b){
  if(baduser.count(b.u_uid))
	  return true;
  return false;
}



