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

#define SQL_ADDR "192.168.1.108"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"

void display(std::list<std::list<std::string> > &msg) ;
void Spilitword(std::string tablename);

DBoperation query(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
DBpool insert;
Parser parser;

/**
 *  @brief Init_Main
 *
 *  @param
 *  @return
 */
void InitMain() {
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root",
     "123456", 50);
  insert.DBinit("use test", connpool);
  query.DBConnect();
  TextSpilt::init_ICTCAL();
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


int main() {
  InitMain();
  cout<<"Program Initialized"<<endl;

  std::set<std::string> tables;
  FilterTables(tables);
  std::set<std::string>::iterator it_beg = tables.begin();
  std::set<std::string>::iterator it_end = tables.end();

  Preprocessor pre;
  query.SetTableName(*it_beg);
  const int ROW_EACH_TIME = 1000;
  int number_all_rows = query.Getcount();
  int number_left_rows = number_all_rows;
  while(number_left_rows > 0){
    std::list<Blog> weibos;
    int n = number_left_rows>ROW_EACH_TIME? ROW_EACH_TIME: number_left_rows;
    query.GetWeiBos(number_all_rows - number_left_rows, n, weibos);
    number_left_rows -= ROW_EACH_TIME;
    for(std::list<Blog>::iterator ib = weibos.begin(), ie = weibos.end();
        ib != ie;
        ib++){
      bool is_good_blog = pre.PerformTactic(*ib);
      if(! is_good_blog){
        PrintBlog(*ib);
      }
    }

  }

//  for (; it_beg != it_end; it_beg++) {
//	  std::cout<<*it_beg<<std::endl;
//  //  Spilitword(*it_beg);
//  }



  std::cout<<"finish the program"<<std::endl;
//  std::cin.ignore();
  return 1;
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




