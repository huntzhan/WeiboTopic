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
//#include"DBdao.h"
#include<iostream>
#include<unistd.h>
#include<time.h>
#include<string>
using namespace std;
//#define THROD 3.0
#define MINVALUE -1
#define TIME
//#define COUNT
//#define BELONT_TOPIC_THROD 2.0
//#define DEBUG_CLUSTER1
//#define CALDIS
//#define PRINTTOPICA

typedef map<string, TopicWord> MAP;
std::vector<std::string> split2(std::string str){
	std::vector<std::string>result;
	std::string mid=str.substr(0,16);
	std::string table=str.substr(16,str.size());
	result.push_back(mid);
	result.push_back(table);
	return result;
}
bool TopicCmp(const Topic &topic1, const Topic &topic2) {
	if (topic1.topic_message_num > topic2.topic_message_num)
		return true;
	return false;
}
bool weibosort(const subword & s1, const subword & s2) {
	if (s1.fre > s2.fre)
		return true;
	return false;
}

void Cluster::CalWordsCooccurrence() {
	std::cout<<"计算贡献度"<<std::endl;
	std::list<OneWeibo> resultword;
	std::list<OneWeibo>::iterator re_w_it;
	std::list<Word>::iterator weiboword_it;
	std::list < std::string > word_coou_oneweibo;
	std::list < std::string >::iterator wco_it_first;
	std::list < std::string >::iterator wco_it_second;
	std::string oneword;
	int count = 0;
	this->dboper->SetTableIndexToZero();
	while (true) {
//		std::cout<<"running in gongxian"<<std::endl;
		if (!this->dboper->ChangeToNextTable(count))
			break;
#ifdef COUNT
		if (count > 10000)
			break;
#endif
		this->dboper->ChangeOneTableCount(count);
		resultword.clear();
		this->dboper->GetMidandText(count,this->dboper->OneTimeReadWeiboNum,resultword);;

		re_w_it = resultword.begin();
		for (; re_w_it != resultword.end(); ++re_w_it) {
			weiboword_it = re_w_it->GetweiboWords()->begin();
			word_coou_oneweibo.clear();
			for (; weiboword_it != re_w_it->GetweiboWords()->end();
					++weiboword_it) {
				oneword = weiboword_it->word;
				if (this->topicword->count(oneword)) {
					word_coou_oneweibo.push_back(oneword);
				}
			}
			wco_it_first=word_coou_oneweibo.begin();
			std::string firstword;
			for (;wco_it_first!=word_coou_oneweibo.end();++wco_it_first) {
				firstword =*wco_it_first;
				std::map<std::string, CooccurrenceWord>::iterator it =
						co_ccur_matrix.find(firstword);
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
				std::map<std::string, double>* map_wordmap =
						it->second.GetWordCooccurrence();
				std::map<std::string, double>::iterator find_it =
						map_wordmap->begin();
				wco_it_second = word_coou_oneweibo.begin();
				std::string secondword;
				for (; wco_it_second != word_coou_oneweibo.end();
						++wco_it_second) {
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

void Cluster::Singlepass() {
#ifdef TIME
	time_t start3;
	start3 = time(NULL);
#endif
	this->CalWordsCooccurrence();
#ifdef TIME
	time_t ends3;
	ends3 = time(NULL);
	std::cout << "计算特征词共现度用时：" << difftime(ends3, start3) << std::endl;
#endif
	this->SetClusterThrod(this->GenClusterThrod() + THROD_ADD);
	std::cout << "CLUSTER_THROD: " << this->CLSTER_THROD << std::endl;
#ifdef DEBUG_CLUSTER1
	printMatrix(this->co_ccur_matrix);
#endif

	MAP::iterator topic_w_it = this->topicword->begin();
	TopicWord firstword = topic_w_it->second;
	Topic topic;
	topic.TopicInit(firstword);
	this->clusterList.push_back(topic);
	topic_w_it++;
	for (; topic_w_it != this->topicword->end(); ++topic_w_it) {

		vector<Topic>::iterator vec_clu_it = this->clusterList.begin();
		double maxDistance = MINVALUE;
		vector<Topic>::iterator belong_clus_it = vec_clu_it;//这里应该是赋值还是指针？？？？
		for (; vec_clu_it != this->clusterList.end(); ++vec_clu_it) {
			double words_distance = Cal_Words_Topic_Distance(*vec_clu_it,
					topic_w_it->second);
			if (maxDistance < words_distance) {
				maxDistance = words_distance;
				belong_clus_it = vec_clu_it;
			}
		}
		if (maxDistance < this->CLSTER_THROD) {
			Topic newTopic;
			newTopic.TopicInit(topic_w_it->second);
			this->clusterList.push_back(newTopic);//这里虽然newTopic是局部变量，但是由于会复制一个新的，有类的时候会动态调用拷贝构造函数
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
	this->MatchWeiboIDToTopic();//match weibo id to topic
	this->ListAllTopicWeiboId();//select the weibo which have more than two words in topic
#ifdef TIME
	time_t ends5;
	ends5 = time(NULL);
	std::cout << "微博对应话题用时：" << difftime(ends5, start5) << std::endl;
#endif
	this->SortTopic();
	this->InsterAllTopicToDatabase();
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
	vector<TopicWord>::iterator clu_it = topic.GetsTopic()->begin();//这里一定要指针？？为什么？
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

void Cluster::MatchWeiboIDToTopic(){
	std::map<std::string ,std::set<std::string> >wordToweiboid;
	std::map<std::string ,std::set<std::string> >::iterator w_t_w_it;
	std::vector<Topic>::iterator topic_list_it=this->clusterList.begin();
	vector<TopicWord>::iterator onetopic_it;
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
		if (count > 10000)
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

void Cluster::ListAllTopicWeiboId() {
	int mycount = 0;
	std::vector<Topic>::iterator clusterList_it = this->clusterList.begin();
	for (; clusterList_it != this->clusterList.end(); ++clusterList_it) {
		ListEveryTopicWeiboId(*clusterList_it);
	}
}
void Cluster::ListEveryTopicWeiboId(Topic &one_topic) {
	vector<TopicWord>::iterator topic_it = one_topic.GetsTopic()->begin();
	std::map<std::string, double>*topic_weibo_id_map =
			one_topic.GetTopicWeiboId();
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
			subword sw(topic_weibo_id_map_it->first,
					topic_weibo_id_map_it->second);
			one_topic.GetWeiboIdList()->push_back(sw);
			one_topic.topic_message_num += 1;
		}
	}
	std::sort(one_topic.GetWeiboIdList()->begin(),
			one_topic.GetWeiboIdList()->end(), weibosort);
	one_topic.GetTopicWeiboId()->clear();
	//将得出的结果存进数据库


}
void Cluster::InsterAllTopicToDatabase(){
	std::vector<Topic>::iterator it = clusterList.begin();
	for(;it!=clusterList.end();++it){
		if(it->topic_message_num==0)continue;
		this->InsertTopicToDatabase(*it);
	}
}
void Cluster::InsertTopicToDatabase(Topic &one_topic) {
	std::cout<<std::endl<<std::endl<<std::endl<<std::endl;
	std::cout<<"该话题下的微博有："<<one_topic.topic_message_num<<" 条"<<std::endl;
	vector<TopicWord> ::iterator topicword_it=one_topic.GetsTopic()->begin();
	for(;topicword_it!=one_topic.GetsTopic()->end();++topicword_it){
		std::cout<<topicword_it->m_sword<<"	";
	}
	std::cout<<std::endl;
	std::vector<subword>::iterator weibo_id_list_it =one_topic.GetWeiboIdList()->begin();

	double weiboid_num = 0.0;
	std::string mid;
	std::string table_name;
	std::string weiboidandtable;
	std::vector<std::string>result;
	Weibo oneweibo;
	for (; weibo_id_list_it != one_topic.GetWeiboIdList()->end(); ++weibo_id_list_it) {
		weiboidandtable=weibo_id_list_it->word;
		result=split2(weiboidandtable);
		weiboid_num = weibo_id_list_it->fre;
		mid=result[0];
		table_name=result[1];
		oneweibo.belongtable=table_name;
		this->dboper->GetOneWeiBo(table_name,mid,oneweibo);
		one_topic.topic_weibo.push_back(oneweibo);//这里考虑直接插入数据库，还是一个话题完之后再插入

//		std::cout<<"topic_weibo："<<oneweibo.mid<<"	"<<oneweibo.text<<std::endl;
	}
	std::cout<<"开始插入数据库"<<std::endl;
	this->dboper->InsertData(one_topic);
	one_topic.topic_weibo.clear();
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

