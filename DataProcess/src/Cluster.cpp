/*
 * cluster.cpp
 *
 *  Created on: 2014年7月31日
 *      Author: hogachen
 */

#include"DEBUG.h"
#include"Cluster.h"
#include "CooccurrenceWord.h"
#include"TopicWord.h"
#include"Topic.h"
#include<iostream>
#include<unistd.h>
#include<time.h>
#include<string>
using namespace std;
#define MINVALUE -1
#define TIME
//#define COUNT

typedef std::map<std::string, TopicWord> MAP;


/*
 * @description：
 *  对话题排序
 */
bool TopicCmp(const Topic &topic1, const Topic &topic2) {
	if (topic1.topic_message_num > topic2.topic_message_num)
		return true;
	return false;
}

/*
 * @description：
 *  根据有多少个话题词出现在微博中，对每个话题下微博进行相关度的排序
 */
bool weibosort(const subword &s1, const subword &s2) {
	if (s1.fre > s2.fre)
		return true;
	return false;
}
/*
 * @description：
 *  遍历每一条微博，首先查询微博下的那些词出现在特征词中，保存到一个list中
 *  再将该list中的词两两共现度加1
 */
void Cluster::CalWordsCoccurrence() {
	std::cout<<"计算贡献度"<<std::endl;

	//存放从数据库中取出的一条微博
	std::list<OneWeibo> resultword;

	std::list<OneWeibo>::iterator re_w_it;
	std::list<Word>::iterator weiboword_it;

	//存放一条微博中出项在特征词集中的词
	std::list < std::string > word_coou_oneweibo;
	std::list < std::string >::iterator wco_it_first;
	std::list < std::string >::iterator wco_it_second;
	std::string oneword;
	int count = 0;
	//这里需要将表的Index重新设置为0
	this->dboper->SetTableIndexToZero();


	while (true) {
		if (!this->dboper->ChangeToNextTable(count))
			break;
#ifdef COUNT
		if (count > 2000)
			break;
#endif
		this->dboper->ChangeOneTableCount(count);

		resultword.clear();
		this->dboper->GetMidandText(count,this->dboper->OneTimeReadWeiboNum,resultword);;

		re_w_it = resultword.begin();
		//找出该微博中出项在特征词集中的词
		for (; re_w_it != resultword.end(); ++re_w_it) {

			weiboword_it = re_w_it->GetweiboWords()->begin();
			word_coou_oneweibo.clear();

			for (; weiboword_it != re_w_it->GetweiboWords()->end();++weiboword_it) {
				oneword = weiboword_it->word;
				if (this->topicword->count(oneword)) {
					word_coou_oneweibo.push_back(oneword);
				}
			}

			wco_it_first=word_coou_oneweibo.begin();
			std::string firstword;
			//对找出的特征词将共现度加到co_ccur_matrix这个共现矩阵中
			for (;wco_it_first!=word_coou_oneweibo.end();++wco_it_first) {
				firstword =*wco_it_first;
				std::map<std::string, CooccurrenceWord>::iterator it =co_ccur_matrix.find(firstword);

				CooccurrenceWord occurword;

				if (it == co_ccur_matrix.end()) {
					CooccurrenceWord occurword;
					std::map<std::string, double>* map_wordmap =
							occurword.GetWordCooccurrence();

					std::map<std::string, double>::iterator find_it;
					wco_it_second=word_coou_oneweibo.begin();
					std::string secondword;

					for (;wco_it_second!=word_coou_oneweibo.end();++wco_it_second) {
						secondword = *wco_it_second;
						if (firstword.compare(secondword) == 0)
							continue;
						find_it = map_wordmap->find(secondword);
						if (find_it != map_wordmap->end()) {
							find_it->second = find_it->second + 1;
						} else {
							map_wordmap->insert(make_pair(secondword, 1.0));
						}
					}

					co_ccur_matrix.insert(make_pair(firstword, occurword));
					continue;
				}


				std::map<std::string, double> *map_wordmap =it->second.GetWordCooccurrence();
				std::map<std::string, double>::iterator find_it =map_wordmap->begin();
				wco_it_second = word_coou_oneweibo.begin();
				std::string secondword;
				for (; wco_it_second != word_coou_oneweibo.end();++wco_it_second) {
					secondword = *wco_it_second;
					if (firstword.compare(secondword) == 0)
						continue;
					find_it = map_wordmap->find(secondword);
					if (find_it != map_wordmap->end()) {
						find_it->second = find_it->second + 1;
					} else {
						map_wordmap->insert(make_pair(secondword, 1.0));
					}
				}
			}
		}
	}
}
/*
 * @description
 *  计算共现度封装了一个计时器
 */
void Cluster::CalConWithTime(){
#ifdef TIME
  time_t start3;
  start3 = time(NULL);
#endif

  this->CalWordsCoccurrence();

#ifdef TIME
  time_t ends3;
  ends3 = time(NULL);
  std::cout << "计算特征词共现度用时：" << difftime(ends3, start3) << std::endl;
#endif
}
/*
 * @description：
 *  一趟聚类的思想
 */
void Cluster::Singlepass() {
#ifdef TIME
  time_t start3;
  start3 = time(NULL);
#endif


	this->SetClusterThrod(this->GenClusterThrod() + THROD_ADD);
	std::cout << "CLUSTER_THROD: " << this->CLSTER_THROD << std::endl;

#ifdef DEBUG_CLUSTER1
	printMatrix(this->co_ccur_matrix);
#endif

	MAP::iterator topic_w_it = this->topicword->begin();
	TopicWord firstword = topic_w_it->second;
	//这里初始化一个词作为一个簇（topic）
	Topic topic;
	topic.TopicInit(firstword);
	this->clusterList.push_back(topic);

	//对于每一个词，计算词在话题列表Topic的距离
	topic_w_it++;
	std::cout<<"聚类前特征词的个数"<<this->topicword->size()<<std::endl;
	for (; topic_w_it != this->topicword->end(); ++topic_w_it) {

		vector<Topic>::iterator vec_clu_it = this->clusterList.begin();
		double maxDistance = MINVALUE;
		vector<Topic>::iterator belong_clus_it = vec_clu_it;//这里应该是赋值还是指针？？？？
		for (; vec_clu_it != this->clusterList.end(); ++vec_clu_it) {
			double words_distance = Cal_Words_Topic_Distance(*vec_clu_it,topic_w_it->second);
			//查看是否有比之前保留的最近的簇更近的簇
			if (maxDistance < words_distance) {
				maxDistance = words_distance;
				belong_clus_it = vec_clu_it;
			}
		}
		//如果该词与该簇距离最近，那么将该词加入该簇
		if (maxDistance < this->CLSTER_THROD) {
			Topic newTopic;
			newTopic.TopicInit(topic_w_it->second);
			this->clusterList.push_back(newTopic);
		} else {
			belong_clus_it->addTopicWord(topic_w_it->second);
		}
	}


#ifdef TIME
	time_t ends4;
	ends4 = time(NULL);
	std::cout << "一趟聚类用时：" << difftime(ends4, start3) << std::endl;
#endif

#ifdef TIME
	time_t start5;
	start5 = time(NULL);
#endif
	//及时将共现矩阵清空
//	this->co_ccur_matrix.clear();
	//match weibo id to topic
	this->MatchWeiboIDToTopic();
	//select the weibo which have more than two words in topic
	this->ListAllTopicWeiboId();

#ifdef TIME
	time_t ends5;
	ends5 = time(NULL);
	std::cout << "微博对应话题用时：" << difftime(ends5, start5) << std::endl;
#endif


	this->SortTopic();
//	this->InsterAllTopicToDatabase();


#ifdef PRINTTOPICA
	printTopic(&this->clusterList);
#endif
}


/*@input
 * 		Topic:话题
 * 		TopicWord:特征词
 * 计算簇与特征词之间的距离
 */
double Cluster::Cal_Words_Topic_Distance(Topic &topic, TopicWord &topic_word) {
	double topic_word_dis = 0.0;
	std::string keyword = topic_word.GetTopicWord();
	std::map<std::string, double>* map_map;
	std::map<string, CooccurrenceWord>::iterator co_it = co_ccur_matrix.find(keyword);
	if (co_it != co_ccur_matrix.end()) {
		map_map = co_it->second.GetWordCooccurrence();
	} else {
		return 0.0;
	}
	list<TopicWord>::iterator clu_it = topic.GetsTopic()->begin();//这里一定要指针？？为什么？
	std::string one_topic_word;
	std::map<std::string, double>::iterator second_it;
	for (; clu_it != topic.GetsTopic()->end(); ++clu_it) {
		one_topic_word = clu_it->GetTopicWord();
		second_it = map_map->find(one_topic_word);
		if (second_it != map_map->end()) {
			topic_word_dis += second_it->second;

		}
	}
	return topic_word_dis / topic.GetsTopic()->size();
}
/*
 * @description：
 *  将话题下的ID索引到话题下
 *  将所有话题的关键词提取出来放到一个MAP中，扫描一遍数据库所有的微博，将微博链接到关键词下，最后查询每个话题，将词下的微博ID再指向话题
 */
void Cluster::MatchWeiboIDToTopic(){
	std::map<std::string ,std::set<std::string> >wordToweiboid;
	std::map<std::string ,std::set<std::string> >::iterator w_t_w_it;
	std::vector<Topic>::iterator topic_list_it=this->clusterList.begin();
	list<TopicWord>::iterator onetopic_it;
	for(;topic_list_it!=this->clusterList.end();++topic_list_it){

		if(topic_list_it->GetsTopic()->size()>=this->BELONG_TOPIC_THROD){
			onetopic_it=topic_list_it->GetsTopic()->begin();
			for(;onetopic_it!=topic_list_it->GetsTopic()->end();++onetopic_it){
				std::set<std::string>onewordset;
				wordToweiboid.insert(make_pair(onetopic_it->m_sword,onewordset));
			}
		}
	}
	std::list<OneWeibo> resultword;
	std::list<OneWeibo>::iterator or_it;
	std::vector<std::string >word_in_map;
	int count=0;
	this->dboper->SetTableIndexToZero();

	while(true){

		if (!this->dboper->ChangeToNextTable(count))
			break;
#ifdef COUNT
		if (count > 2000)
			break;
#endif
		this->dboper->ChangeOneTableCount(count);

		resultword.clear();
		this->dboper->GetMidandText(count, this->dboper->OneTimeReadWeiboNum,resultword);

		or_it=resultword.begin();
		for(;or_it!=resultword.end();++or_it){

			std::list<Word>::iterator it= or_it->words.begin();
			for(;it!=or_it->words.end();++it){
				if((w_t_w_it=wordToweiboid.find(it->word))!=wordToweiboid.end()){
					w_t_w_it->second.insert(or_it->MID+this->dboper->table_name);
				}
			}
		}
	}
	//re_match the  weibo id to topic
	topic_list_it=this->clusterList.begin();
	for (; topic_list_it != this->clusterList.end(); ++topic_list_it) {

		if (topic_list_it->GetsTopic()->size() >= this->BELONG_TOPIC_THROD) {
			onetopic_it = topic_list_it->GetsTopic()->begin();
			for (; onetopic_it != topic_list_it->GetsTopic()->end(); ++onetopic_it) {
				if((w_t_w_it=wordToweiboid.find(onetopic_it->m_sword))!=wordToweiboid.end()){
					onetopic_it->word_to_weiboid_list=w_t_w_it->second;//复制拷贝
				}
			}
		}
	}

}
/*
 * @description：
 *  统计话题下微博ID中每条微博出项了多少个关键词，根据此对微博相关度进行排序，同时删除阈值小于BELONG_TOPIC_THROD（3）的都不要
 */
void Cluster::ListAllTopicWeiboId() {
	int mycount = 0;
	std::vector<Topic>::iterator clusterList_it = this->clusterList.begin();
	for (; clusterList_it != this->clusterList.end(); ++clusterList_it) {
		ListEveryTopicWeiboId(*clusterList_it);
	}
}
void Cluster::ListEveryTopicWeiboId(Topic &one_topic) {
	list<TopicWord>::iterator topic_it = one_topic.GetsTopic()->begin();
	std::map<std::string, double> *topic_weibo_id_map =one_topic.GetTopicWeiboId();
	std::map<std::string, double>::iterator topic_weibo_id_map_it;
	one_topic.topic_message_num = 0;


	for (; topic_it != one_topic.GetsTopic()->end(); ++topic_it) {
		std::set<std::string>::iterator topicword_weibolist_it =
				topic_it->GetWordToWeiboidList()->begin();
		for (; topicword_weibolist_it
				!= topic_it->GetWordToWeiboidList()->end(); ++topicword_weibolist_it) {
			std::string weiboid = *topicword_weibolist_it;
			topic_weibo_id_map_it = topic_weibo_id_map->find(weiboid);
			if (topic_weibo_id_map_it != topic_weibo_id_map->end()) {
				topic_weibo_id_map_it->second = topic_weibo_id_map_it->second + 1;
			} else {
				topic_weibo_id_map->insert(make_pair(weiboid, 1.0));
			}
		}
	}



	topic_weibo_id_map_it = topic_weibo_id_map->begin();

	for (; topic_weibo_id_map_it != topic_weibo_id_map->end(); ++topic_weibo_id_map_it) {
		if (topic_weibo_id_map_it->second >= this->BELONG_TOPIC_THROD) {
			subword sw(topic_weibo_id_map_it->first,topic_weibo_id_map_it->second);

			one_topic.GetWeiboIdList()->push_back(sw);
			one_topic.topic_message_num += 1;
		}
	}

	one_topic.GetTopicWeiboId()->clear();
	std::sort(one_topic.GetWeiboIdList()->begin(),
			one_topic.GetWeiboIdList()->end(), weibosort);

	//及时清除内存
	if(one_topic.topic_message_num<=this->MIN_TOPIC_MESSAGE_NUM){
	    one_topic.weibo_id_list.clear();
	    std::vector<subword>(one_topic.weibo_id_list).swap(one_topic.weibo_id_list);
	}
	//将得出的结果存进数据库
}


void Cluster::SortTopic() {
	std::sort(this->clusterList.begin(), this->clusterList.end(), TopicCmp);
}
std::vector<int> Cluster::GenRandomValue() {
	int MAX = this->GetCooccurrence()->size() - 1;
	if (MAX > RAND_SIZE)
		randsize = RAND_SIZE;
	else
		randsize = MAX;
	srand((unsigned) time(NULL));
	std::vector<int> v_i;
	for (int i = 0; i < randsize; i++) {
		v_i.push_back(rand() % MAX);
	}
	return v_i;
}

/**
 * @description：
 *  计算一趟聚类阈值
 */
double Cluster::GenClusterThrod() {
	double tempthrod = 0.0;
	std::vector<std::string> tempvec;
	std::map<std::string, CooccurrenceWord>::iterator it =
			this->GetCooccurrence()->begin();
	for (; it != this->GetCooccurrence()->end(); ++it) {
		tempvec.push_back(it->first);
	}

	std::vector<int> tempintvec = this->GenRandomValue();
	for (int i = 0; i < tempintvec.size(); ++i) {
		int index = tempintvec[i];
		it = this->GetCooccurrence()->find(tempvec[index]);
		tempthrod += it->second.GetWordCooccurrence()->begin()->second;
	}
	return tempthrod / randsize;
}
void Cluster::EraseCo_ccur_matrix() {
  std::vector<Topic>::iterator it = this->clusterList.begin();
  std::map<std::string, CooccurrenceWord>::iterator map_it;
  std::map<std::string,TopicWord>::iterator t_it;
  std::list<TopicWord>::iterator topicword_it;
  std::cout<<"删除元素"<<std::endl;
  std::cout<<"删除之前map size为： "<<this->co_ccur_matrix.size()<<std::endl;
  std::cout<<"删除之前topicword size为： "<<this->topicword->size()<<std::endl;
  int count=0;
  for (; it != this->clusterList.end(); ++it) {

    if (it->topic_message_num >= this->MIN_TOPIC_MESSAGE_NUM) {
      count++;
      this->clusterListTemp.push_back(*it);
      //删除co_ccur_matrix中已经形成话题的关键词
      topicword_it = it->GetsTopic()->begin();
      //输出所有词
      for (; topicword_it != it->GetsTopic()->end(); ++topicword_it) {
        std::string topicwordstr = topicword_it->m_sword;
        map_it = this->co_ccur_matrix.find(topicwordstr);
        t_it=this->topicword->find(topicwordstr);
        if (map_it != this->co_ccur_matrix.end()) {
          this->co_ccur_matrix.erase(map_it);
        }
        if(t_it!=this->topicword->end()){
          this->topicword->erase(t_it);
        }
      }
    }

  }
  std::cout<<"原来话题个数;"<<count<<std::endl;
  std::cout<<"删除之后topicword size为： "<<this->topicword->size()<<std::endl;
  std::cout<<"删除之后map size为： "<<this->co_ccur_matrix.size()<<std::endl;
  this->clusterList.clear();
  std::cout<<"再次聚类"<<std::endl;
  //在聚类之前降低阈值
  this->THROD_ADD=0;
  this->BELONG_TOPIC_THROD=2;
  this->Singlepass();

  std::cout<<"合并话题"<<std::endl;
  this->MergeClusterList();
}
void Cluster::MergeClusterList(){
  std::vector<Topic>::iterator it = this->clusterList.begin();
  int count2=0;
  std::cout<<"第一次temp大小"<<this->clusterListTemp.size()<<std::endl;
  for(;it!=this->clusterList.end();++it){
     if(it->topic_message_num>=this->MIN_TOPIC_MESSAGE_NUM)
     {
       count2++;
       this->clusterListTemp.push_back(*it);
     }
  }
  std::cout<<"合并后temp大小"<<this->clusterListTemp.size()<<std::endl;
  std::cout<<"第二次聚类后话题个数;"<<count2<<std::endl;
  this->clusterList.clear();
  std::copy(this->clusterListTemp.begin(),this->clusterListTemp.end(),std::back_inserter(this->clusterList));
  this->SortTopic();
  //最后的清除共现矩阵
  this->co_ccur_matrix.clear();
}
