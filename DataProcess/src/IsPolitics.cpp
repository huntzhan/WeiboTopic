/*
 * IsPolitic.cpp
 *
 *  Created on: 2014年8月12日
 *      Author: jeffzhuang
 *      The second author:hogachen
 *  description：
 *      政治类话题提取
 */


#include "IsPolitics.h"
#define DEBUG_POLITICS
/*
 * 获得关键词的词表
 */
void IsPolitics::GetkeyMap(){
	std::ifstream infile("./keyword.txt");
	if(!infile){
		std::cerr<<"input file error"<<std::endl;
	}
	while(!infile.eof()){
		std::string temp;
		infile>>temp;
//		std::cout<<"keyword:  "<<temp<<std::endl;
		this->politic_word_map.insert(make_pair(temp,0.0));
		infile>>temp;
	}
}


void IsPolitics::SplitWeiboSplit(std::string &str,std::vector<WeiboWord> &result) {

  std::vector<std::string> goodWordstemp;
  boost::split(goodWordstemp, str, boost::is_any_of(" "));
  std::string  wordstr,wordpro;
  std::vector<std::string>::iterator it_word = goodWordstemp.begin();
  std::vector<std::string>::iterator end_word = goodWordstemp.end();
  for (int i = 0; i < goodWordstemp.size() - 1; i++) {

    wordstr = goodWordstemp[i];
    i++;
    wordpro = goodWordstemp[i];
    WeiboWord word(wordstr,wordpro,1.0);
    result.push_back(word);
  }
}



void IsPolitics::AddKeyToMapWithProperty(WeiboWord &word) {
  std::string key = word.wordstr;
  std::map<string, WeiboWord>::iterator it;

  if (key.size() >= 4) {

    it = this->keyword_map.find(key);
    if (it == this->keyword_map.end()) {
      WeiboWord weiboword(key, word.wordpro, 1.0);
      this->keyword_map.insert(make_pair(key, weiboword));
    } else {
      it->second.SetFrequency(it->second.wordfre + 1);
    }
  }
}


void IsPolitics::GetEveryWordInOneTopicByWordProperty(Topic &onetopic) {
  this->topic_word_size=0;
  std::vector<WeiboWord>::iterator or_it;
  std::list<Weibo>::iterator t_w_it;
  t_w_it = onetopic.GetTopicWeibo()->begin();
  for(;t_w_it != onetopic.GetTopicWeibo()->end();++t_w_it){
    std::vector<WeiboWord> resultword;
    this->SplitWeiboSplit(t_w_it->spilt,resultword);

    or_it = resultword.begin();
    for (; or_it != resultword.end(); ++or_it) {
      this-> topic_word_size++;
      this->AddKeyToMapWithProperty(*or_it);
    }
  }

}
void IsPolitics::ReadBayeModel(ClassificationInfo classification, std::string filename){
  std::ifstream infile(filename.c_str());
  int flag=0;
  std::string temp1;
  std::string temp2;
  double value;
  while(!infile.eof()){
    if(flag==0){
      infile>>temp1;
      classification.article_num=atoi(temp1.c_str());
      infile>>temp1;
      classification.word_num=atoi(temp1.c_str());

    }
    infile>>temp1;
    infile>>temp2;
    value=atof(temp2.c_str());
    classification.GetWrodFre()->insert(make_pair(temp1,value));
  }
  infile.close();
}
void IsPolitics::IsTopicPoliticsByBaye(Topic &onetopic)
{
  this->GetEveryWordInOneTopicByWordProperty(onetopic);
  std::map<std::string,double>::iterator other_it;
  std::map<std::string,double>::iterator politic_it;
  std::string key;
  double other_classification_pro;
  double politic_classification_pro;
  //两种方案：1.直接计算词的后验概率  2.计算后验概率的同时将词的权值（词频等）也计算进去
  std::map<std::string, WeiboWord>::iterator it = this->GetKeyWordMap()->begin();
  for (; it != this->GetKeyWordMap()->end(); ++it) {
    key = it->first;
    other_it = this->other_classification.word_fre.find(key);
    if (other_it != this->other_classification.word_fre.end()) {
      other_classification_pro *= log(other_it->second);//*it->second.wordfre/100//由于词频太大会导致乘完之后后验概率失效;
    }
    //计算在政治类中的后验概率
    politic_it = this->other_classification.word_fre.find(key);
    if (politic_it != this->politic_classification.word_fre.end()) {
      politic_classification_pro *= log(politic_it->second);//同上
    }


    //判断属于哪一个类别
    if(politic_classification_pro > other_classification_pro){
      onetopic.isPolitic=1;
    }
  }

}

/*
 * 处理的主要过程 通过累加该topic下包含关键词的数量
 */
void  IsPolitics::IsTopicPolitics(Topic &onetopic){
  //这里要记得初始化为0
  this->keyword_map.clear();
  this-> topic_word_size=0;

  this->GetEveryWordInOneTopicByWordProperty(onetopic);
  this->GetkeyMap();
  std::string onetopicword;
  double wrodpro=0.0;
  double topic_score=0.0;
  std::map<std::string, WeiboWord>::iterator it = this->keyword_map.begin();
  for(;it != this->keyword_map.end(); ++it){
    onetopicword = it->first;
    if( this->politic_word_map.count(onetopicword)){
      std::cout<<"ispolitic: "<<onetopicword<<"  "<<it->second.wordfre<<std::endl;
      topic_score+=it->second.wordfre;
    }
  }
  double rate = topic_score/(double)this->topic_word_size;
  if( rate >= 0.2){
    onetopic.isPolitic=1;
  }else{
    onetopic.isPolitic=0;
  }
  std::cout<<"topic_score "<<topic_score<<" rate: "<<rate<<" SIZE: "<<this->topic_word_size<<std::endl;

}
void IsPolitics::InitIsPolitics(){
      this->ReadBayeModel(this->other_classification,"other_classification.txt");
      this->ReadBayeModel(this->politic_classification, "politic_classification.txt");
}



