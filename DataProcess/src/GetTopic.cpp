/*
 * GetTopic.cpp
 *
 *  Created on: 2014年8月3日
 *      Author: hogachen
 */

#include "GetTopic.h"
#include "DEBUG.h"
#include "Weibo.h"
#include<time.h>
#include<math.h>
#include<algorithm>
//#define COUNT
#define TIME
/*@description：
 *  排序函数，需要注意不能放在类的内部
 *@input
 *  键值对：key1&key2
 */
bool SortCmp(const PAIR &key1, const PAIR &key2) {
	if (key1.second.m_dFrequency > key2.second.m_dFrequency)
		return true;
	return false;
}
/*
 * @description：
 *  扫描数据库一遍来提取特征词
 */
void GetTopic::GetEveryWordInCurrentHourByWordProperty() {
  std::list<OneWeibo> resultword;
  std::list<OneWeibo>::iterator or_it;

  //临时统计变量
  int count = 0;
  int num = 0;


  while (true) {
    /*
     * 这里用了dboper做数据库表之间的切换
     */
    if (!this->dboper->ChangeToNextTable(count))
      break;
#ifdef COUNT
    if (count > 2000)
    break;
#endif
    this->dboper->ChangeOneTableCount(count);

    resultword.clear();

//    this->dboper->GetMidandText(count, this->dboper->OneTimeReadWeiboNum,resultword);
    this->dboper->GetMidandTextAndSplit(count, this->dboper->OneTimeReadWeiboNum,resultword);
    or_it = resultword.begin();
    for (; or_it != resultword.end(); ++or_it) {
      num++;
      std::list<Word>::iterator it = or_it->words.begin();
      for (; it != or_it->words.end(); ++it) {
        this->AddKeyToMapWithProperty(this->m_topic_word, *it);
      }
    }
  }


  std::cout << "处理数据：" << num << "条" << std::endl;
}




/* @description：
 *  对每一个单词做判断，只保留v（动词）、vn（动名词）、ns（地名）、n（名词）、un（名词）
 * @input
 *  word：               保持词的词性和词本身
 *  filterMap：存放特征词的map引用
 */
void GetTopic::AddKeyToMapWithProperty(map<std::string, TopicWord> &filterMap, Word &word) {
	std::string key = word.word;
	map<string, TopicWord>::iterator it;

	if (word.word.size() >= 4 && (word.is_tag_word==true||word.proper.compare("v") == 0
			|| word.proper.compare("vn") == 0 || word.proper.compare("n") == 0
			|| word.proper.compare("un") == 0)) {

		it = filterMap.find(key);
		if (it == filterMap.end()) {
		  double value=1.0;
		  if(word.is_tag_word==true)value=30.0;
			TopicWord topicword(key, value);
//			if(word.is_tag_word==true)std::cout<<topicword.m_sword<<"  "<<topicword.m_dFrequency<<std::endl;
			filterMap.insert(make_pair(key, topicword));
		} else {
		  if(word.is_tag_word==true)it->second.SetFrequency(it->second.GetFrequency() + 30);
			it->second.SetFrequency(it->second.GetFrequency() + 1);
		}
	}

	//当存放特征词的map size超过某个阈值（100000）时删除里面出次数少于10的词
	if (filterMap.size() > TOPICMAPTHROD) {
		this->DeleteElementsBelowThrod(filterMap);
		std::cout<<"delete the key that below 10...."<<std::endl;
	}
}


/*
 * @description：
 *  删除map的函数
 */
void GetTopic::DeleteElementsBelowThrod(map<std::string, TopicWord> &filterMap) {
	map<string, TopicWord>::iterator map_it = filterMap.begin();

	for (; map_it != filterMap.end(); ++map_it) {
		if (map_it->second.m_dFrequency < 10) {
			filterMap.erase(map_it);
		}
	}

}

void GetTopic::GenTopicWordByFrequency() {
#ifdef TIME
	time_t start;
	start = time(NULL);
#endif


	this->GetEveryWordInCurrentHourByWordProperty();


#ifdef TIME
	time_t ends;
	ends = time(NULL);
	std::cout << "获取特征词用时：" << difftime(ends, start) << std::endl;
#endif


	this->TopicWordSort();
}


/*@description：
 * 对提取出来的特征词进行词频排序
 */
void GetTopic::TopicWordSort() {

	map<string, TopicWord>::iterator mapIt;
	vector<PAIR> sort_vec;
	for (mapIt = this->m_topic_word.begin(); mapIt != this->m_topic_word.end(); ++mapIt) {
		sort_vec.push_back(make_pair(mapIt->first, mapIt->second));
	}

	sort(sort_vec.begin(), sort_vec.end(), SortCmp);

#ifdef DEBUG2
	printVector(sort_vec);
#endif
	//提取指定的特征词数量（10000）个
	int topicWordNum = this->m_topic_word.size();
	this->m_topic_word.clear();

	if (topicWordNum > TOPIC_WORD_NUM){
	  topicWordNum = TOPIC_WORD_NUM;
	}

	vector<PAIR>::iterator v_it = sort_vec.begin();
	int count = 0;
	while (v_it != sort_vec.end() && count < topicWordNum) {
		string key = v_it->first;
		TopicWord value_topic = v_it->second;
		if (v_it->second.m_dFrequency> 10) {
			this->m_topic_word.insert(make_pair(key, value_topic));
		}
		++v_it;
		++count;
	}
}


