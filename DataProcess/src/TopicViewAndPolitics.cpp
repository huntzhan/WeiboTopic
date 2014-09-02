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
  this->ispo.InitIsTrash();
  this->dboper->QueryIfOneDayTableExistAndCreate();
  std::vector<Topic>::iterator it = this->clusterList->begin();
  int topicnum=0;

  std::list<TopicWord>::iterator topicword_it ;
  for(;it!=this->clusterList->end();++it){
    if(it->topic_message_num<this->MIN_TOPIC_MESSAGE_NUM)
    {
      break;
    }
    topicnum++;
    this->GetOneTopicWeiboByBatch(*it,topicnum);
  }
}

void print10weibo(std::list<Weibo> &weibolist){
  std::cout<<"话题下的5条微博如下： "<<std::endl<<std::endl;
  int counter=0;
  std::list<Weibo>::iterator it = weibolist.begin();
  for(;it!=weibolist.end();++it){
    if(++counter>5)break;
    std::cout<<std::endl;
    std::cout<<it->text<<std::endl;
  }
}

//打印输出信息
void TopicViewAndPolitics::printMessage(Topic &one_topic, int topicnum){

    std::cout<<std::endl<<std::endl<<std::endl<<std::endl;
    std::cout<<"话题TOP"<<topicnum<<std::endl;
    std::cout<<"该话题下的微博有："<<one_topic.topic_message_num<<" 条"<<std::endl;
    list<TopicWord> ::iterator topicword_it=one_topic.GetsTopic()->begin();
    std::cout<<std::endl;
    std::cout<<"话题热议词如下"<<std::endl;
    for(;topicword_it!=one_topic.GetsTopic()->end();++topicword_it){
      std::cout<<topicword_it->m_sword<<" ";
    }
    std::cout<<std::endl;
}

//当数据量达到一定量时及时查询数据库
void TopicViewAndPolitics::QueryWeiboIntime( std::map<string ,std::list<std::string> >&table_to_weibo,
    std::list<Weibo> &weibocontentlist){

  std::map<string ,std::list<std::string> >::iterator map_it;
  map_it=table_to_weibo.begin();
  for(;map_it!=table_to_weibo.end();++map_it){
    std::string table=map_it->first;
    this->dboper->SelectOnetableByBatch(table,map_it->second,weibocontentlist);
  }
}
//批量查询数据库
void TopicViewAndPolitics::GetOneTopicWeiboByBatch(Topic &one_topic, int topicnum){

  //一个表下面的多条微博，一次批量查询一个表
  if(one_topic.topic_message_num<=this->MIN_TOPIC_MESSAGE_NUM)return;
  printMessage(one_topic,  topicnum);

  std::string mid;
  std::string table_name;
  std::string weiboidandtable;
  std::vector<std::string>result;

  //将微博mid按表分开，以便批量查询数据库
  std::map<string ,std::list<std::string> >table_to_weibo;
  std::map<string ,std::list<std::string> >::iterator map_it;

  //计数变量
  int politic_counter=0;
  int totalnum=0;

  //是否是第一次插入数据库的标志位
  int flag=0;
  //政治类话题是否处理过
  int processed_politic=0;

  std::vector<subword>::iterator weibo_id_list_it =one_topic.GetWeiboIdList()->begin();
  for (; weibo_id_list_it != one_topic.GetWeiboIdList()->end(); ++weibo_id_list_it) {

    weiboidandtable=weibo_id_list_it->word;
    result=split2(weiboidandtable);
    mid=result[0];
    table_name=result[1];

    map_it=table_to_weibo.find(table_name);
    if(map_it!=table_to_weibo.end()){
      map_it->second.push_back(mid);
    }else{
      std::list<std::string> one_table_weibo_list;
      one_table_weibo_list.push_back(mid);
      table_to_weibo.insert(make_pair(table_name,one_table_weibo_list));
    }
    //当量达到50条时或者达到话题微博数时，进行话题概要提取和判断是否为政治类
    politic_counter++;
    if(politic_counter==this->TOPICVIEW_WEIBO_NUM||
        politic_counter==one_topic.topic_message_num){
      //如果处理过了，就不再处理
      if(processed_politic==0){
        QueryWeiboIntime(table_to_weibo, one_topic.topic_weibo);
        this->tw.GenOneTopicView(one_topic);
        this->ispo.IsTopicPoliticsByBaye(one_topic);
        std::cout<<"是否为政治类话题 ? : "<<one_topic.isPolitic<<std::endl<<std::endl;
        std::cout<<"是否为垃圾类话题 ? : "<<one_topic.isTrash<<std::endl<<std::endl;
        print10weibo(one_topic.topic_weibo);
        processed_politic=1;
        //这里需要删除，不然下面会重复查询插入
        one_topic.topic_weibo.clear();
//        std::cout<<"in 50 :"<< one_topic.topic_weibo.size()<<std::endl;
      }
    }
    //当微博量达到5000条时，查询数据库，同时清除table_to_weibo和one_topic.topic_weibo
    totalnum++;
    if(totalnum>=10000){
      QueryWeiboIntime(table_to_weibo, one_topic.topic_weibo);
      this->dboper->InsertData(one_topic,flag);
      std::cout<<"插入数据库一次，插入了 "<<totalnum<<" 条微博"<<std::endl;
      table_to_weibo.clear();
      one_topic.topic_weibo.clear();
      totalnum=0;
      flag=1;
    }
  }
  //最后要将没有满5000条的部分插入数据库
  //最后没有满足5000条的也需要查询数据库插入
  QueryWeiboIntime(table_to_weibo, one_topic.topic_weibo);
  //暂时切换为输出数据到本地文本
//  this->SaveFileToLocal(one_topic,topicnum);
  this->dboper->InsertData(one_topic,flag);
  one_topic.topic_weibo.clear();
  std::vector<subword>(one_topic.weibo_id_list).swap(one_topic.weibo_id_list);
}
std::string TopicViewAndPolitics::intTostring(int a){
  char tem[10];
  sprintf(tem,"%d",a);
  std::string res(tem);
  return res;
}
void TopicViewAndPolitics::SaveFileToLocal(Topic &one_topic,int id){
  std::list<Weibo>::iterator it = one_topic.topic_weibo.begin();
  std::string text;
  std::string idstr=intTostring(id);
  std::string filename = "../TrashFilter/"+idstr+".txt";
  std::ofstream outfile(filename.c_str());
  if(!outfile){
    std::cout<<"打开文件失败!"<<std::endl;
    return;
  }
  for(;it!= one_topic.topic_weibo.end();++it) {
    text=it->text;
    outfile<<text<<std::endl;
  }
  outfile.close();
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

/*
 * @description：
 *  将一个话题插入数据库，先将话题的主要信息（包括话题微博数，话题主要观点，话题关键词）插入OneDayTopic表，获取插入时的ID
 *  再将话题下的微博查询出来，插入数据库，由于最大的话题下的微博数会超过10万，一次读入再插入数据库会占用太多内存，所以分批（达到1000）
 *  就插入一次
 */
/*
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
//      break;
    }
    flagnum++;
    //设置为每查询到1000条微博就批量插入一次数据库
    if(flagnum==1000){
      this->dboper->InsertData(one_topic,flag);
      std::cout<<"插入数据库一次，插入了 "<<readnum<<" 条微博"<<std::endl;
      flag=1;flagnum=0;
      std::cout<<"size: "<<one_topic.weibo_id_list.size()<<std::endl;
    }
    if(readnum>=3001)break;
  }
  this->dboper->InsertData(one_topic,flag);
  one_topic.topic_weibo.clear();
  //释放内存
  one_topic.weibo_id_list.clear();
  std::vector<subword>(one_topic.weibo_id_list).swap(one_topic.weibo_id_list);
}*/
