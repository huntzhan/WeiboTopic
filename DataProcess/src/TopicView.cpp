/*
 * TopicView.cpp
 *
 *  Created on: 2014年8月11日
 *      Author: hogachen
 */

#include"TopicView.h"
#include"DEBUG.h"
#include<vector>
#include<map>
#include<string>
#include<algorithm>
//#define NUM_OF_SUB_WORD 100
#define TOPICVIEW
//#define SUBDEBUG

void TopicView::GenAllTopicView(){
	std::vector<Topic>::iterator it = this->clusterList->begin();
	for(;it!=this->clusterList->end();++it){
		this->GenOneTopicView(*it);
	}
#ifdef TOPICVIEW
//		printTopicView(*it);
		printTopic(this->clusterList,this->dbdao);
#endif

}
void TopicView::GenOneTopicView(Topic & onetopic){
	std::vector<std::string>* topic_weibo_id=onetopic.GetWeiboIdList();
	std::vector<std::string>::iterator twid_it=topic_weibo_id->begin();
	std::string weiboid;
	Weibo oneweibo;
	for(;twid_it!=topic_weibo_id->end();++twid_it){
		weiboid=*twid_it;
		this->dbdao->GetEveryWeiboFromDatabase(weiboid,oneweibo);
		std::vector<std::string>::iterator v_it = oneweibo.GetWords()->begin();
		std::string twoword;
		std::map<std::string,double>*zhutici_map=onetopic.GetMainIdea();
		std::map<std::string,double>::iterator ztm_it;
#ifdef SUBDEBUG
		std::cout<<"running1 "<<std::endl;
#endif SUBDEBUG
		for(;v_it!=oneweibo.GetWords()->end();++v_it){
			if(v_it>=oneweibo.GetWords()->end()-1)break;
			twoword=*v_it+*(v_it+1);
//			std::cout<<"twoword: "<<twoword<<std::endl;
#ifdef SUBDEBUG
		std::cout<<"running 2"<<std::endl;
#endif SUBDEBUG
			ztm_it=zhutici_map->find(twoword);
			if(ztm_it==zhutici_map->end())
				zhutici_map->insert(make_pair(twoword,1.0));
			else{
				ztm_it->second+=1;
			}
			if(v_it>=oneweibo.GetWords()->end()-2)continue;
			twoword+=*(v_it+2);
			ztm_it=zhutici_map->find(twoword);
			if(ztm_it==zhutici_map->end())
				zhutici_map->insert(make_pair(twoword,1.0));
			else{
				ztm_it->second+=1;
			}
		}
	}
	this->SortSubTopicMap(onetopic);
}
//void TopicView::FindWordInMap(std::string key,std::map<std::string ,double>*map){
//
//}
void TopicView::SortSubTopicMap(Topic &onetopic){
	std::vector<PAIRS>vec_sort;
	std::map<std::string,double>*zhutici_map=onetopic.GetMainIdea();
	std::map<std::string,double>::iterator it =zhutici_map->begin();
	for(;it!=zhutici_map->end();++it){

		vec_sort.push_back(make_pair(it->first,it->second));
	}
	int count=0;
	int num_of_sub_word=NUM_OF_SUB_WORD;
	if(num_of_sub_word<zhutici_map->size()){
		num_of_sub_word = zhutici_map->size();
	}
	zhutici_map->clear();
	std::sort(vec_sort.begin(),vec_sort.end(),SubTopicSort);
	std::vector<PAIRS>::iterator sort_vec_it=vec_sort.begin();
	for(;sort_vec_it!=vec_sort.end();++sort_vec_it){
		++count;
		if(count>num_of_sub_word)break;
//		zhutici_map->insert(*sort_vec_it);
		subword sw(sort_vec_it->first,sort_vec_it->second);
//		std::cout<<"push back!"<<std::endl;
		onetopic.GetSubWordList()->push_back(sw);
	}
}
bool SubTopicSort(const PAIRS & key1,const PAIRS & key2){
	if(key1.second > key2.second) return true;
	return false;
}
