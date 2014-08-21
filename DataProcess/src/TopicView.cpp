/*
 * TopicView.cpp
 *
 *  Created on: 2014年8月11日
 *      Author: hogachen
 */

#include"TopicView.h"
#include"TopicWord.h"
#include"DEBUG.h"
#include<vector>
#include<map>
#include<string>
#include<algorithm>
#define TOPICVIEW

void TopicView::GenAllTopicView() {
  std::vector<Topic>::iterator it = this->clusterList->begin();
  for (; it != this->clusterList->end(); ++it) {
    this->GenOneTopicView(*it);
  }
#ifdef TOPICVIEW
//	printTopicView(*it);
//  printTopic(this->clusterList, this->dbdao, this->BELONG_TOPIC_THROD);
#endif
}



void TopicView::GenOneTopicView(Topic &onetopic) {
  std::list<Weibo> topic_weibo;
  std::list<Weibo>::iterator twid_it = onetopic.GetTopicWeibo()->begin();
  std::string weiboid;
  Weibo oneweibo;
  for (; twid_it != onetopic.GetTopicWeibo()->end(); ++twid_it) {

    oneweibo=*twid_it;
    //用原微博文本做主题词提取
//    std::string text = oneweibo.text;
//    std::vector<std::string> splittext=this->dboper->stringSplitToVector(text,this->splitLen);
    //用分好词的文本做主提词提取
    std::string split = oneweibo.spilt;
    std::vector<std::string>splittext=SplitWeiboSplit(split);
    std::vector<std::string>::iterator v_it = splittext.begin();

    std::string twoword;
    std::string threeword;
    std::map<std::string, double>*zhutici_map = onetopic.GetMainIdea();
    std::map<std::string, double>::iterator ztm_it;

    for (; v_it != splittext.end(); ++v_it) {

      if (v_it >= splittext.end() - 2)
        break;
      twoword = *v_it + *(v_it + 1)+*(v_it + 2);
      ztm_it = zhutici_map->find(twoword);
      if (ztm_it == zhutici_map->end())
        zhutici_map->insert(make_pair(twoword, 1.0));
      else {
        ztm_it->second += 1;
      }


      if (v_it >= splittext.end() - 3)
        continue;
      threeword = twoword+*(v_it + 3);
//      std::cout<<"three word : "<<threeword<<std::endl;
      ztm_it = zhutici_map->find(threeword);
      if (ztm_it == zhutici_map->end())
        zhutici_map->insert(make_pair(threeword, 1.0));
      else {
        ztm_it->second += 1;
      }
    }

  }
  this->SortSubTopicMap(onetopic);
  this->SelectMainIdeaWithTopicWord(onetopic);
  printTopicView(onetopic);
  onetopic.GetMainIdea()->clear();
}
/*
 * @description：
 *  在提取出单位关键词按词频排序后，再将查询前10个词中是否有出现话题关键词，如果有的话，就将其权值加大
 */
void TopicView::SelectMainIdeaWithTopicWord(Topic &onetopic){
  int main_idea_count=0;
  std::string mainideaword;
  int findIndex=100000;

  std::map<std::string,double>::iterator map_it;
  std::cout<<"running"<<std::endl;
  std::list<subword>::iterator m_i_it=onetopic.GetSubWordList()->begin();
  for(;m_i_it != onetopic.GetSubWordList()->end();++m_i_it){
    onetopic.GetMainIdea()->insert(make_pair(m_i_it->word,m_i_it->fre));
  }
  std::cout<<onetopic.GetSubWordList()->size()<<std::endl;
  m_i_it=onetopic.GetSubWordList()->begin();
  for(;m_i_it != onetopic.GetSubWordList()->end();++m_i_it){
    if(main_idea_count++>40)break;
    mainideaword = m_i_it->word;
//    std::cout<<main_idea_count<<"main idea"<<std::endl;
    list<TopicWord>::iterator t_w_it = onetopic.GetsTopic()->begin();

    for(;t_w_it != onetopic.GetsTopic()->end();++t_w_it){
//      std::cout<<"int "<<t_w_it->m_sword<<"  "<<mainideaword<<std::endl;
      std::string word = t_w_it->m_sword;
      findIndex=mainideaword.find(word);
      if(findIndex<mainideaword.size() && findIndex>=0){
//        std::cout<<"before : "<<map_it->second<<std::endl;
        map_it=onetopic.GetMainIdea()->find(mainideaword);
        map_it->second+=5;

//        std::cout<<"add  quanzhi:"<<mainideaword<<"   "<<map_it->second<<std::endl;
      }
    }
  }

  this->SortSubTopicMap(onetopic);
}

void TopicView::SortSubTopicMap(Topic &onetopic) {
  std::vector<PAIRS> vec_sort;
  std::map<std::string, double>*zhutici_map = onetopic.GetMainIdea();
  std::map<std::string, double>::iterator it = zhutici_map->begin();
  for (; it != zhutici_map->end(); ++it) {

    vec_sort.push_back(make_pair(it->first, it->second));
  }
  int count = 0;
  int num_of_sub_word = NUM_OF_SUB_WORD;
  if (num_of_sub_word < zhutici_map->size()) {
    num_of_sub_word = zhutici_map->size();
  }
  zhutici_map->clear();
  onetopic.GetSubWordList()->clear();
  std::sort(vec_sort.begin(), vec_sort.end(), SubTopicSort);
  std::vector<PAIRS>::iterator sort_vec_it = vec_sort.begin();
  for (; sort_vec_it != vec_sort.end(); ++sort_vec_it) {
    ++count;
    if (count >= num_of_sub_word - 1)
      break;
    subword sw(sort_vec_it->first, sort_vec_it->second);
    onetopic.GetSubWordList()->push_back(sw);
  }
}



bool SubTopicSort(const PAIRS & key1, const PAIRS & key2) {
  if (key1.second > key2.second)
    return true;
  return false;
}



std::vector<std::string> TopicView::SplitWeiboSplit(std::string &str) {
  std::vector<std::string> result;
  std::vector<std::string> goodWordstemp;
  boost::split(goodWordstemp, str, boost::is_any_of(" ")); //分割

  std::vector<std::string>::iterator it_word = goodWordstemp.begin();
  std::vector<std::string>::iterator end_word = goodWordstemp.end();
  for (int i = 0; i < goodWordstemp.size() - 1; i++) {
//    Word word;
//    word.word = goodWordstemp[i];
    result.push_back(goodWordstemp[i]);
    i++;
//    word.proper = goodWordstemp[i];

  }
  return result;
}

