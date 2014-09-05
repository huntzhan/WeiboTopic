/*
 * coverage.cpp
 *
 *  Created on: 2014-8-25
 *      Author: jeffzhuang
 */
#include "coverage.h"
bool SortCmp(const pair<string, TopicTag> &key1,
		const pair<string, TopicTag> &key2);
bool CountSortCmp(const TopicCount &key1, const TopicCount &key2);



void CCoverage::GetTable(std::list<std::string> &table) {
	std::list<std::string>::iterator it_table = table.begin();
	std::list<std::string>::iterator end_table = table.end();
	for (; it_table != end_table; it_table++) {
		std::string name = *it_table;
		if (name[0] == 'F') {
			FilterTable.push_back(name);
		} else if (name[0] == 'T') {
			TopicTable.push_back(name);
		} else if (name[0] == 'O') {
			OneDayTopic = name;
		}
	}
}
//获得包含tag主要观点
void CCoverage::GetTag() {
	list<Coverage::WEIBO> weibos;
	int sum = 0;
	for (int i = 0; i < 11; i++) {
		std::string table = FilterTable[i];
		//	std::cout << table << std::endl;
		dboper->SetTableName(table);
		string ziduan = "text";
		dboper->GetTagText(weibos, ziduan);
		///循环读取微博
		list<Coverage::WEIBO>::iterator it_weibos = weibos.begin();
		list<Coverage::WEIBO>::iterator end_weibos = weibos.end();
		for (; it_weibos != end_weibos; it_weibos++) {
			std::string content = it_weibos->content;
			int posStart = 0, posEnd = 0;
			posStart = content.find('#', 0);
			if (posStart >= 0) {
				posEnd = content.find('#', posStart + 1);
				if (posEnd > posStart) {
					string tag = content.substr(posStart + 1,
							posEnd - posStart - 1);
					TopicTag topic_tag;
					topic_tag.mids.push_back(it_weibos->mid);
					topic_tag.num = 1;
					topic_tag.tag = tag;
					//     std::cout<<tag<<std::endl;
					sum++;
					std::pair<std::map<std::string, TopicTag>::iterator, bool> ret =
							topics.insert(make_pair(tag, topic_tag));
					if (!ret.second) {
						ret.first->second.num++;
						ret.first->second.mids.push_back(it_weibos->mid);
					}

				}
				continue;
			}
			posStart = content.find("＃", 0);
			if (posStart >= 0) {
				posEnd = content.find("＃", posStart + 1);
				if (posEnd > posStart) {
					string tag = content.substr(posStart + strlen("＃"),
							posEnd - posStart - strlen("＃"));
					TopicTag topic_tag;
					topic_tag.mids.push_back(it_weibos->mid);
					topic_tag.num = 1;
					topic_tag.tag = tag;
					//  std::cout<<tag<<std::endl;
					sum++;
					std::pair<std::map<std::string, TopicTag>::iterator, bool> ret =
							topics.insert(make_pair(tag, topic_tag));
					if (!ret.second) {
						ret.first->second.num++;
						ret.first->second.mids.push_back(it_weibos->mid);
					}

				}
				continue;
			}

		}
	}

}

bool SortCmp(const pair<string, TopicTag> &key1,
		const pair<string, TopicTag> &key2) {
	if (key1.second.num > key2.second.num)
		return true;
	return false;
}
/**
 * 对包含tag的主要观点进行统计排序
 */
void CCoverage::SortTag() {
	disCount = 800;

	std::map<std::string, TopicTag>::iterator it_map = topics.begin();
	std::map<std::string, TopicTag>::iterator end_map = topics.end();
	for (; it_map != end_map; it_map++) {
		SortVector.push_back(make_pair(it_map->first, it_map->second));
	}
	sort(SortVector.begin(), SortVector.end(), SortCmp);
	int count = SortVector.size() > disCount ? disCount : SortVector.size();

	for (int i = 0; i < count; i++) {
		string tag = SortVector[i].first;
		unsigned int hash_value = sim.BlogHash(tag.c_str());
		//std::cout<<tag<<"|"<<SortVector[i].second.num<<"|"<<sim.BlogHash(SortVector[i].first.c_str())<<std::endl;
		TagHash.insert(make_pair(hash_value, tag));
	}
}
/**
 * 读取标准的热门话题
 */
void CCoverage::GetHourTopic(const char * path) {
	ifstream infile(path);
	int i = 0; // 没有意义
	string temp;
	if (!infile) {
		std::cerr
				<< "################  can not find the topic from sina use the default file###############"
				<< std::endl;
		infile.open("/tmp/JavaProjct-jinfa/HopTopic/HourTopic/2014_09_02_12");
	}
	while (!infile.eof()) {
		i++;
		infile >> temp;
//		cout<<temp<<endl;
		TagHash.insert(make_pair(i, temp));
	}
	TagHash.erase(TagHash.find(i - 1));
	cout << TagHash.size() << endl;
	infile.close();
}

/**
 * 从数据库OneDayTopic提取主要观点与及ID号
 */
void CCoverage::GetTopicMainIdea(const std::string tablename) {
	ofstream outfile("/tmp/JavaProjct-jinfa/HopTopic/ourTopics.txt");	//写文件
	vector<pair<int, string> > result;
	dboper->SetTableName(tablename);
	dboper->getTopicMainIdea(result);
	vector<pair<int, string> >::iterator it_result = result.begin();
	vector<pair<int, string> >::iterator end_result = result.end();
	for (; it_result != end_result; it_result++) {
		vector<string> goodWordstemp;
		vector<string> goodWord;
		std::string MainIdeaStr = it_result->second;
//		  std::cout<<MainIdeaStr<<endl;
		boost::trim(MainIdeaStr);
		boost::split(goodWordstemp, MainIdeaStr, boost::is_any_of(" "));  //分割
		MainTopic maintopic;
		maintopic.id = it_result->first;
		maintopic.subTopic = goodWordstemp;
		topicMainIdea.push_back(maintopic);
		outfile << MainIdeaStr;
		outfile << "\n";

	}
	outfile.close();
}

void CCoverage::MatchTopic() {
	vector<MainTopic>::iterator it_topic = topicMainIdea.begin();
	vector<MainTopic>::iterator end_topic = topicMainIdea.end();
	for (; it_topic != end_topic; it_topic++) {
		vector<string>::iterator it_subTopic = it_topic->subTopic.begin();
		vector<string>::iterator end_subTopic = it_topic->subTopic.end();
		for (; it_subTopic != end_subTopic; it_subTopic++) {

			unsigned int v = sim.BlogHash(it_subTopic->c_str());
			vector<unsigned int> hashVector;
			sim.HammingValuesWithinDistance(v, SimhashThrethod, hashVector);
			for (unsigned int i : hashVector) {
				if (TagHash.count(i)) {
					map<unsigned, string>::iterator it = TagHash.find(i);
					printf(
							"微博热门话题: %12s   OneDayTopic  %30s     the id is %d \n",
							it->second.c_str(), it_subTopic->c_str(),
							it_topic->id);
					//std::cout<<"微博热门话题:"<<std::setw(40)<<std::left<<it->second<<std::setw(30)<<std::left<<"OneDayTopic"<<std::setw(30)<<*it_subTopic<<std::setw(30)<<"topic ID "<<std::setw(30)<<it_topic->id<<std::endl;
				}
			}
		}
	}
}


/**
 * 读取文件获得文件的话题量
 */
void CCoverage::mGetSearchCount(void) {
	ifstream infile("/tmp/JavaProjct-jinfa/HopTopic/topicCount.txt");
	if (!infile) {
		std::cerr << "input topic file error" << std::endl;
	}
	string temp;
	long count;
	int i = 0;
	vector<pair<string, long> > vec;
	while (!infile.eof()) {
		infile >> temp;
		infile >> count;
		i++;
		if (i <= 4) {
			vec.push_back(make_pair(temp, count));
			if (i == 4) {
				TopicCount topiccount;
				topiccount.hottopic = "";
				topiccount.count = 0;
				topiccount.topic = vec;
				TopicSearchCount.push_back(topiccount);
				vec.clear();
				i = 0;
			}
		}
	}
}


void CCoverage::GetSearchCount(const std::string tablename){
	GetTopicMainIdea(tablename);
	system("ls -al /etc/passwd /etc/shadow");
	mGetSearchCount();
	SearchCountSort();
}

bool CountSortCmp(const TopicCount &key1, const TopicCount &key2) {
	if (key1.count > key2.count)
		return true;
	return false;
}
/**
 * 根据搜索量对聚类出的话题进行排序
 */
void CCoverage::SearchCountSort() {
	vector<TopicCount>::iterator it_searchcount = TopicSearchCount.begin();
	vector<TopicCount>::iterator end_count = TopicSearchCount.end();
	long maxlong = 0, index = 0;
	for (; it_searchcount != end_count; it_searchcount++) {
		maxlong = 0;
		index = 0;
		for (unsigned int i = 0; i < it_searchcount->topic.size(); i++) {
			if (it_searchcount->topic[i].second > maxlong) {
				maxlong = it_searchcount->topic[i].second;
				index = i;
			}
		}
		it_searchcount->hottopic = it_searchcount->topic[index].first;
		it_searchcount->count = maxlong;
	}

	sort(TopicSearchCount.begin(), TopicSearchCount.end(), CountSortCmp);
	it_searchcount = TopicSearchCount.begin();
	end_count = TopicSearchCount.end();
	for (; it_searchcount != end_count; it_searchcount++) {
		cout << it_searchcount->hottopic << "-------" << it_searchcount->count
				<< endl;
	}
}
/*
 * 输入两个短文本进行
 * 返回jaccard系数
 */
double CCoverage::Jaccard(std::string str1, std::string str2) {
	std::wstring wstr = TextSpilt::StringToWide(str1);
	std::wstring wstr2 = TextSpilt::StringToWide(str2);
	int lenght1 = wstr.length();
	int length2 = wstr2.length();
	int splitLen = 1;
	std::wstring substr, strtemp;
	int count = 0;
	for (int i = 0; i < wstr.length(); i += splitLen) {
		substr = wstr.substr(i, splitLen);
		int pos = wstr2.find(substr, 0);
		if (pos >= 0 && pos < wstr2.length()) {
			wstr2.replace(pos, 1, L"");
			count++;
		}
	}
	return double((double) count / (lenght1 + length2 - count));
}
/**
 * 使用jaccard系统来进行匹配对应的是否为主要话题命中
 */
void CCoverage::_MatchTopicByJaccard() {
	vector<MainTopic>::iterator it_topic = topicMainIdea.begin();
	vector<MainTopic>::iterator end_topic = topicMainIdea.end();
	for (; it_topic != end_topic; it_topic++) {
		vector<string>::iterator it_subTopic = it_topic->subTopic.begin();
		vector<string>::iterator end_subTopic = it_topic->subTopic.end();
		for (; it_subTopic != end_subTopic; it_subTopic++) {
			map<unsigned, string>::iterator it_tag = TagHash.begin();
			map<unsigned, string>::iterator end_tag = TagHash.end();
			for (; it_tag != end_tag; it_tag++) {
				//比较两个话题是否相似
				double ja = Jaccard(*it_subTopic, it_tag->second);
				if (ja >= JaccardThrethod) {
//					cout << ja << endl;
//					printf(
//							"微博热门话题: %-50s   OneDayTopic  %-50s     the id is %d \n",
//							it_tag->second.c_str(), it_subTopic->c_str(),
//							it_topic->id);

					if (!matchTopic.count(it_topic->id)) {
						MainTopic maintopic;
						maintopic.id = it_topic->id;
						maintopic.matchstring = it_tag->second;
						maintopic.subTopic = it_topic->subTopic;
						matchTopic.insert(make_pair(it_topic->id, maintopic));
					}

				}
			}
		}
	}
}

/**
 *  输入 tablename就是程序传递进来要处理的表
 *  OneDayTopic对应的topic表名字
 *  输出的文件在/tmp/JavaProjct-jinfa/OutPut/目录下
 *
 */
void CCoverage::MatchTopicByJaccard(const std::string tablename,
		const std::string OneDayTopic) {
//	 ///提取包含tag的文本
//	 GetTag();
//	 ///对tag进行排序
//	 SortTag();
	std::string hotTopic = TramTime(tablename);
	string path = "/tmp/JavaProjct-jinfa/HopTopic/HourTopic/" + hotTopic;
	GetHourTopic(path.c_str());
	///获得聚类的主要观点
	GetTopicMainIdea(OneDayTopic);
	///jaccard相似度匹配
	_MatchTopicByJaccard();
	DisplayMatch();
	OutPutResult();

	cout << "准确率 ： " << double(matchTopic.size()) / topicMainIdea.size()
			<< "   覆盖率 :" << double(matchTopic.size()) / TagHash.size() << endl;
}

void CCoverage::MatchTopicBySimHash() {
	///提取包含tag的文本
	GetTag();
	///对tag进行排序
	SortTag();
	///获得聚类的主要观点
	GetTopicMainIdea("OneDayTopic_20140831");
	///simhash相似度匹配
	MatchTopic();
}

void CCoverage::DisplayMatch() {
	map<int, MainTopic>::iterator it_match = matchTopic.begin();
	map<int, MainTopic>::iterator end_match = matchTopic.end();
	for (; it_match != end_match; it_match++) {
		cout << "topic ID " << it_match->second.id
				<< "-----------------匹配对应的话题  ：          "
				<< it_match->second.matchstring << endl;
	}
}

void CCoverage::OutPutResult() {
	string outputfile="/tmp/JavaProjct-jinfa/OutPut/match"+time+".txt";
	string inputfile="/tmp/JavaProjct-jinfa/OutPut/unmatch"+time+".txt";
	ofstream matchOutfile(outputfile.c_str());	//写文件
	ofstream unmatchOutfile(inputfile.c_str());//写文件

	map<int, MainTopic>::iterator it_match = matchTopic.begin();
	map<int, MainTopic>::iterator end_match = matchTopic.end();
	for (; it_match != end_match; it_match++) {
		map<int, MainTopic>::iterator it_id = matchTopic.find(
				it_match->second.id);
		if (it_id != matchTopic.end()) {
			string line;
			for (string maintopic : it_id->second.subTopic) {
				line += maintopic + " ";
			}
			line += it_match->second.matchstring + "\n";
			matchOutfile << line;
		}
	}
	matchOutfile << double(matchTopic.size()) / topicMainIdea.size() << " "
			<< double(matchTopic.size()) / TagHash.size();
	int flag_match = 0;
	map<unsigned, string>::iterator it_tag = TagHash.begin();
	map<unsigned, string>::iterator end_tag = TagHash.end();
	for (; it_tag != end_tag; it_tag++) {
		map<int, MainTopic>::iterator it_topic = matchTopic.begin();
		map<int, MainTopic>::iterator end_topic = matchTopic.end();
		flag_match = 0;
		for (; it_topic != end_topic; it_topic++) {
			if (it_topic->second.matchstring == it_tag->second) {
				flag_match = 1;
				break;
			}
		}
		if (flag_match == 0) {
			unmatchOutfile << it_tag->second;
			unmatchOutfile << "\n";
		}
	}
}

string CCoverage::TramTime(const std::string table) {
	char result[80];
	string tablename = "";
	long pos = table.find("140", 0);
	string substr = table.substr(pos, strlen("1408179600"));
	cout << substr << endl;
	time=substr;
	long time = atol(substr.c_str());
	time_t t = (time_t) time;
	struct tm *ptr;
	ptr = localtime(&t);
	sprintf(result, "%4d_%02d_%02d_%02d", ptr->tm_year + 1900, ptr->tm_mon + 1,
			ptr->tm_mday, ptr->tm_hour);
	tablename = result;
	return tablename;
}

