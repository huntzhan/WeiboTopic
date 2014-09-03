///*
// * IsPolitic.cpp
// *
// *  Created on: 2014年8月12日
// *      Author: jeffzhuang
// *      The second author:hogachen
// *  description：
// *      政治类话题提取
// */
//
//
//#include "IsPolitic.h"
//#define DEBUG_POLITICS
///*
// * 获得关键词的词表
// */
//void IsPolitics::GetkeyMap(){
//	std::ifstream infile("./keyword.txt");
//	if(!infile){
//		std::cerr<<"input file error"<<std::endl;
//	}
//	while(!infile.eof()){
//		std::string temp;
//		infile>>temp;
//		keyMap.insert(make_pair(temp,0));
//		infile>>temp;
//	}
//}
//
//void IsPolitics::GetWordMap(){
//
//
////	std::set<std::string> stopwords;
////	std::string lineword;
////	std::vector<std::string> vec;
////	std::ifstream infile("./test.txt");
////	if(!infile){
////		std::cerr<<"input file error"<<std::endl;
////	}
////	init_ICTCAL();
////	MakeStopSet(stopwords);
////	while(getline(infile,lineword)){
////		goodWordArticle(lineword,stopwords,vec);
////		std::vector<std::string>::iterator it_vec=vec.begin();
////		std::vector<std::string>::iterator end_vec=vec.end();
////		for(;it_vec!=end_vec;it_vec++){
////			std::pair<std::map<std::string,int>::iterator,bool > ret=wordMap.insert(std::make_pair((std::string)(*it_vec),1));
////			if(!ret.second)
////			++ret.first->second;
////		}
////		vec.clear();
////	}
//
//}
///********************************************************************************8
// * 处理的主要过程 通过累加该topic下包含关键词的数量
// */
//double IsPolitics::Process(){
//
//	//这个要调试
//	GetkeyMap();
////	GetWordMap();
//
//
//	std::map<std::string,int>::iterator it_word;
//	int occurs;
//	double score=0;
//	std::map<std::string,int>::iterator it_key=keyMap.begin();
//	std::map<std::string,int>::iterator end_key=keyMap.end();
//	for(;it_key!=end_key;it_key++){
//
//#ifdef DEBUG_POLITICS
////		std::cout<<it_key->first<<"||"<<it_key->second<<"  ";
//#endif
//		score+=it_key->second;
//		it_key->second=0;
//	}
//
//
//	return score;
//}
//
//
//void IsPolitics::IsAllTopicPolitic(DBoperation *dboper,std::vector<Topic> *clusterList){
//	this->dboper=dboper;
//	this->clusterList=clusterList;
//	outfile.open("./politic.log");
//	std::vector<Topic>::iterator it_list=(*clusterList).begin();
//	std::vector<Topic>::iterator end_list=(*clusterList).end();
//	for(;it_list!=end_list;it_list++){
//		if(IsOneTopicPolitic(*it_list))
//			politic_cluster_list.push_back(&(*it_list));
//	}
//
//}
//
///*
// * 处理一个促就是一个topic的微博内容
// */
//bool IsPolitics::IsOneTopicPolitic(Topic & onetopic){
//
//	static int topics=0;
//	std::vector<subword> *weibo_id_list;
//	weibo_id_list=onetopic.GetWeiboIdList();
//	//获得微博该TOPIC下的所有ID号
//	std::vector<subword>::iterator it_MID=weibo_id_list->begin();
//	std::vector<subword>::iterator end_MID=weibo_id_list->end();
//	std::map<std::string,int>::iterator end_keyMap=keyMap.end();
//	for(;it_MID!=end_MID;it_MID++){
//		Weibo weibo;
//		dbdao->GetEveryWeiboFromDatabase((*it_MID).word,weibo);
//		std::vector<std::string> *weibo_ID=weibo.GetWords();
//		std::vector<std::string>::iterator it_word=weibo_ID->begin();
//		std::vector<std::string>::iterator end_word=weibo_ID->end();
//		for(;it_word!=end_word;it_word++){
//			//收集单条微博的内容
//			std::map<std::string,int>::iterator it_keyMap=keyMap.find(*it_word);
//			if(it_keyMap!=end_keyMap){
//				it_keyMap->second++;
//			}
//		}
//	}
//
//	double score=Process();
//#ifdef DEBUG_POLITICS
//	topics++;
//	std::cout<<topics<<"   sorce is"<<score<<std::endl;
//#endif
//	if(score<threhold){
//		return false;
//	}
//	return true;
//}
//
//IsPolitics::IsPolitics(DBoperation *dboper,std::vector<Topic> *clusterList){
//		this->dboper=dboper;
//		this->clusterList=clusterList;
//}
