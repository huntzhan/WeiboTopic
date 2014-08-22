/******************************************************************************* 
 Filename   : process.cpp
 Version    : ver 1.0
 Author     :zhuangjf@gmail.com
 Date       : 2014/07/30
 Description:
 History    :
 *******************************************************************************/
#include "db/DBoperation.h"
#include "db/DBpool.h"
#include "db/connection_pool.h"
#include "db/model.h"
#include "split/parser.h"
#include "split/Textspilt.h"
#include "tactic/preprocessor.h"
#include <map>
#include "simhash/simhash.h"

#define SQL_ADDR "192.168.1.108"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"


void display(std::list<std::list<std::string> > &msg) ;
void Spilitword(std::string tablename);
void count_the_user(list<Blog> &weibos);

Parser parser;
DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
DBpool insert;

/**
 *  @brief Init_Main
 *
 *  @param
 *  @return
 */
void InitMain() {
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root",
     "123456", 2);
  insert.DBinit("use test", connpool);
  query.DBConnect();
  TextSpilt::init_ICTCAL();
}

void FilterOneTable(string table, list<Blog> &return_weibo_list) {
  query.SetTableName(table);
  int number_all_rows = query.Getcount();
  int number_left_rows = number_all_rows;
  int ROW_EACH_TIME = 2000;
  Preprocessor pre;
  while(number_left_rows > 0){
    std::list<Blog> weibos;
    int n = number_left_rows>ROW_EACH_TIME? ROW_EACH_TIME: number_left_rows;
    /// print IO Operation
    cout<< "Get blogs from db: " << n << endl;
    query.GetWeiBos(number_all_rows - number_left_rows, n, weibos);
    number_left_rows -= ROW_EACH_TIME;
    /// start blogs filtering
    for(auto &blog : weibos) {
      bool is_good_blog = pre.PerformTactic(blog);
      if(is_good_blog){
        return_weibo_list.push_back(blog);
        // PrintBlog(*ib);
      }
    }
  }
}

int main() {
  InitMain();
  cout<<"Program Initialized"<<endl;

  std::list<std::string> tables;
  query.GetTables(tables);
  /// FilterTables(tables);
  /// get tables from to be processed
  for(const auto &table : tables){
    query.SetTableName(table);
    int number_all_rows = query.Getcount();
    cout<<"###Start Filtering One Table: " << number_all_rows << endl;
    list<Blog> goodweibos;
    FilterOneTable(table, goodweibos);
    cout<<"###Blogs in One Table After Filtering: " << goodweibos.size() << "/" << number_all_rows <<endl;
    /**
     * Perform DB Insertion Operation here
     */
  }

  std::cout<<"Program has finished"<<std::endl;
  return EXIT_SUCCESS;
}

bool SortCmp(const std::pair<string,int> &key1 ,const std::pair<string ,int> &key2 ){
	if (key1.second > key2.second)
		return true;
	return false;
}
/*
 *计算1个小时内重复发微博的数目
 *
 */
void count_the_user(list<Blog> &weibos) {
	list<string> badweibolist;
	SimHash simhash;
	std::map<string, int> users;
	std::cout << weibos.size() << endl;
	for (std::list<Blog>::iterator ib = weibos.begin(), ie = weibos.end();
			ib != ie; ib++) {
		std::list<string> m_list;
		std::pair<std::map<std::string, int>::iterator, bool> ret =
				users.insert(std::make_pair((*ib).u_uid, 1));
		if (!ret.second) {
			++ret.first->second;  /// inc the user count
		}

	}
	std::map<string, int>::iterator it_map = users.begin();
	std::map<string, int>::iterator end_map = users.end();
	std::vector<std::pair<string, int> > vector_sort;
	for (; it_map != end_map; it_map++) {
		vector_sort.push_back(make_pair(it_map->first, it_map->second));
	}
    sort(vector_sort.begin(),vector_sort.end(),SortCmp);
	///获得前100的用户重复
	std::string badUID = " ";
	unsigned countsum=vector_sort.size()>1000?1000:vector_sort.size();
	std::cout<<countsum<<std::endl;
	for (int i_count = 0; i_count < countsum;i_count++) {
         if(vector_sort[i_count].second<3){
        	 break;
         }
		std::cout << "uid----" << vector_sort[i_count].first << "----num---"
				<< vector_sort[i_count].second << std::endl;
		vector<pair<unsigned int, std::string>> hashvector;
		vector<std::string> mids;
		for (std::list<Blog>::iterator ib = weibos.begin(), ie = weibos.end();
				ib != ie; ib++) {
			if ((*ib).u_uid == vector_sort[i_count].first) {
				hashvector.push_back(
						make_pair(simhash.BlogHash(ib->m_content.c_str()),ib->m_content));
				        mids.push_back(ib->m_mid);
				// std::cout<<ib->u_fans<<"  "<<ib->u_followees<<ib->m_content<<std::endl;
			}
			if ((*ib).u_uid == badUID) {
				///删除坏的用户的微博
				std::cout<<badUID<<std::endl;
				std::cout<<ib->m_content<<std::endl;
				ib=weibos.erase(ib);
			}

		}
		badUID = vector_sort[i_count].first;
		vector<int> hashresult;
		int count = 0;
		//计算用户在一个小时内所发的微博的两两hash值
		for (unsigned int i_first = 0; i_first < hashvector.size(); i_first++) {
			for (unsigned int j_second = i_first; j_second < hashvector.size(); j_second++) {
				hashresult.push_back(
						simhash.Calculate_Distance(hashvector[i_first].first,
								hashvector[j_second].first));
				count++;
			}
		}
		int sum = 0;
		for (unsigned int i = 0; i < hashresult.size(); i++) {
			sum += hashresult[i];
		}
		///如果hash的平均值小于3，就是说这个小时内所发的微博内容几乎一样
		if (count != 0) {
			if (double((double) sum / count) < 3) {
				for (unsigned int i = 0; i < hashvector.size(); i++) {
					std::cout << hashvector[i].second << std::endl;
					badweibolist.push_back(mids[i]);
					///把改ID记录下来 下次循环的时候就删除该用户所发的微博
				}
			}
			else{
				badUID=" ";
			}
		}
	    std::cout << "-------------------" << double((double) sum / count)<< "---------------------------" << std::endl;
//		std::cout << "################################################"
//				<< std::endl;
	}

}



/**************************************************************************
 * 显示vector二维里面的内容
 */
void display(std::list<std::list<std::string> > &msg) {
  std::list<std::list<std::string> >::iterator it_first = msg.begin();
  std::list<std::list<std::string> >::iterator end_first = msg.end();
  for (; it_first != end_first; it_first++) {
    std::list<std::string>::iterator id_second = it_first->begin();
    std::list<std::string>::iterator end_second = it_first->end();
    for (; id_second != end_second; id_second++) {
      std::cout << *id_second << std::endl;
    }
    std::cout << "################################################" << std::endl;
  }
}
/**
 * 这个是主要处理的 先从数据库里面提取数据，然后分词，最后插入
 */
void Spilitword(std::string tablename) {
  std::list<std::list<std::string> > resultList;
  query.SetTableName(tablename);
  /// insert.SetTableName(tablename);
  /// 建立数据库的表
  /// insert.CreateTable();
  long count = query.Getcount();
  std::cout << tablename << " " << count << std::endl;

  time_t startT, endT;
  double total;
  for (long pos = 0; pos < count - 1000;) {
    startT = time(NULL);
    query.GetText(pos, 1000, resultList);
    std::list<std::list<std::string> >::iterator it_first =
        resultList.begin();
    std::list<std::list<std::string> >::iterator end_first =
        resultList.end();
    std::vector<INSERT_DATA> insert_datas;
    for (; it_first != end_first; it_first++) {

      std::string rawtext = (*it_first).back();
      if (rawtext.empty())
        continue;
      std::string fenci;
      std::vector<Word> words;
      /// ICTCLAS and stopwrods parsing
      /// parser.LexicalAnalysis(rawtext, words);
      std::vector<Word>::iterator it_word = words.begin();
      std::vector<Word>::iterator end_word = words.end();
      for (; it_word != end_word; it_word++) {
        fenci.append(it_word->word + " " + it_word->proper + " ");
      }
      INSERT_DATA insertdata;
      insertdata.mid = (*it_first).front();
      insertdata.text = (*it_first).back();
      insertdata.spilt = fenci;
      insert_datas.push_back(insertdata);
    }
    /// 
    /// insert.DB_insertData(insert_datas);
    pos = pos + 1000;
    endT = time(NULL);
    total = difftime(endT, startT);
    std::cout << "the runing time is " << total << std::endl;
    resultList.clear();
    std::cout << "----------------------------------" << "finish"
        << "------------------------------------------" << pos
        << std::endl;
  }
}

/**
 *  @brief FilterTables return the tables to be processed
 *
 *  @param
 *  @return
 */
 void FilterTables(std::set<std::string> &settable) {
  /// get tables from to be processed
  std::list<std::string> tables;
  query.GetTables(tables);
  /// get tables that have already been processed
  std::list<std::string> inserttables;
  insert.GetTables(inserttables);

  /// remove the duplicated tables
  std::list<std::string>::iterator it_table = tables.begin();
  std::list<std::string>::iterator end_table = tables.end();
  for ( ; it_table != end_table; it_table++) {
    settable.insert(*it_table);
  }
  std::list<std::string>::iterator it_inserttable = inserttables.begin();
  std::list<std::string>::iterator end_inserttable = inserttables.end();
  for ( ; it_inserttable != end_inserttable; it_inserttable++) {
    if (settable.count(*it_inserttable))
      settable.erase(*it_inserttable);
  }
  std::cout<<settable.size()<<std::endl;
}


