/*
 * TopicViewAndPolitics.cpp
 *
 *  Created on: 2014年8月24日
 *      Author: Administrator
 */
#include"TopicViewAndPolitics.h"
/*
 * @description：
 *  切分mid和微博表字符串
 * @input
 *  str：带切分的mid+belongTable格式字符串
 * @output：
 *  vector<string> result : result[0]为mid，result[1]为tablename
 */
std::vector<std::string> split2(std::string str){
  std::vector<std::string>result;
  std::string mid=str.substr(0,16);
  std::string table=str.substr(16,str.size());
  result.push_back(mid);
  result.push_back(table);
  return result;
}

//将所有的话题插入数据库，这里考虑优化为多线程
void TopicViewAndPolitics::InsterAllTopicToDatabase(){
  this->tw.InitTopicView(this->dboper,this->clusterList,this->NUM_OF_SUB_WORD,4);

  this->ispo.InitIsPolitics();
  std::vector<Topic>::iterator it = this->clusterList->begin();
  for(;it!=this->clusterList->end();++it){
    this->InsertTopicToDatabase(*it);
  }
}
void print10weibo(std::list<Weibo> &weibolist){
  std::cout<<"话题下的5条微博如下： "<<std::endl<<std::endl;
  int counter=0;
  std::list<Weibo>::iterator it = weibolist.begin();
  for(;it!=weibolist.end();++it){
    if(++counter>=5)break;
    std::cout<<std::endl;
    std::cout<<it->text<<std::endl;
  }
}
/*
 * @description：
 *  将一个话题插入数据库，先将话题的主要信息（包括话题微博数，话题主要观点，话题关键词）插入OneDayTopic表，获取插入时的ID
 *  再将话题下的微博查询出来，插入数据库，由于最大的话题下的微博数会超过10万，一次读入再插入数据库会占用太多内存，所以分批（达到1000）
 *  就插入一次
 */
void TopicViewAndPolitics::InsertTopicToDatabase(Topic &one_topic) {
  if(one_topic.topic_message_num<=this->MIN_TOPIC_MESSAGE_NUM)return;
  std::cout<<std::endl<<std::endl<<std::endl<<std::endl;
  std::cout<<"该话题下的微博有："<<one_topic.topic_message_num<<" 条"<<std::endl;
  list<TopicWord> ::iterator topicword_it=one_topic.GetsTopic()->begin();
  std::cout<<std::endl;
  std::cout<<"话题热议词如下"<<std::endl;
  for(;topicword_it!=one_topic.GetsTopic()->end();++topicword_it){
    std::cout<<topicword_it->m_sword<<" ";
  }
  std::cout<<std::endl;


  double weiboid_num = 0.0;
  std::string mid;
  std::string table_name;
  std::string weiboidandtable;
  std::vector<std::string>result;
  Weibo oneweibo;
  int readnum=0;
  int flagnum=0;
  int flag=0;

  std::vector<subword>::iterator weibo_id_list_it =one_topic.GetWeiboIdList()->begin();
  for (; weibo_id_list_it != one_topic.GetWeiboIdList()->end(); ++weibo_id_list_it) {
    weiboidandtable=weibo_id_list_it->word;
    result=split2(weiboidandtable);
    weiboid_num = weibo_id_list_it->fre;
    mid=result[0];
    table_name=result[1];
    oneweibo.belongtable=table_name;
    //这里是最耗时的
    this->dboper->GetOneWeiBo(table_name,mid,oneweibo);
    one_topic.topic_weibo.push_back(oneweibo);//这里是一个话题完之后再插入
    readnum++;
    //达到TOPICVIEW_WEIBO_NUM（50）条微博时，就开始提取话题概要，同时判断该话题是否为政治类
    if(readnum==this->TOPICVIEW_WEIBO_NUM||readnum == one_topic.topic_message_num){
      this->tw.GenOneTopicView(one_topic);
      this->ispo.IsTopicPoliticsByBaye(one_topic);
      std::cout<<"是否为政治类话题 ? : "<<one_topic.isPolitic<<std::endl<<std::endl;
      print10weibo(one_topic.topic_weibo);
      break;
    }
    flagnum++;
    //设置为每查询到1000条微博就批量插入一次数据库
    if(flagnum==1000){
//      this->dboper->InsertData(one_topic,flag);
      std::cout<<"插入数据库一次，插入了 "<<readnum<<" 条微博"<<std::endl;
      flag=1;flagnum=0;
      std::cout<<"size: "<<one_topic.weibo_id_list.size()<<std::endl;
    }
  }
//  this->dboper->InsertData(one_topic,flag);
  one_topic.topic_weibo.clear();
  //释放内存
  one_topic.weibo_id_list.clear();
  std::vector<subword>(one_topic.weibo_id_list).swap(one_topic.weibo_id_list);
}

/*
 * @description：
 *  以下是准备用多线程查询话题下的子话题模块
 */


/*
class Message{
public:
  int start;
  int end;
};
void *Cluster::InsertTopicThread(void * args )
{
    Message* message = (Message*)args;
    int start=message->start;
    int end = message->end;
    int skipcounter=0;
    std::vector<Topic>::iterator clus_it=this->clusterList.begin();
    while(skipcounter++<start)clus_it++;

    while(skipcounter++<end){
      InsertTopicToDatabase(*clus_it);
      clus_it++;
    }
    pthread_exit(NULL);
}

void Cluster::InsterAllTopicToDatabase(){

  this->tw.InitTopicView(this->dboper,&this->clusterList,this->NUM_OF_SUB_WORD,4);
  this->ispo.InitIsPolitics();

  std::list<pthread>thread_id_list;
  int counter=0;
  std::vector<Topic>::iterator it = clusterList.begin();
  for(;it!=10;it+=2){//clusterList.end()先10个话题测试
    pthread_t id;
    int ret;
    Message message;
    message.start=counter;
    counter+=2;
    message.end=counter;
    ret=pthread_create(&id,NULL,InsertTopicThread,message);
    if(ret!=0)
    {
        printf("Create pthread error!\n");
        return ;
    }
    thread_id_list.push_back(id);
//    this->InsertTopicToDatabase(*it);
  }
  //让线程在这里等待
  std::list<pthread>::iterator t_id_it=thread_id_it.begin();
  for(;t_id_it!=thread_id_it.end();++thread_id_it){
    pthread_join(*t_id_it,NULL);
  }
}
*/

