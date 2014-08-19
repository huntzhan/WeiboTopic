/******************************************************************************* 
 Filename   : process.cpp
 Version    : ver 1.0
 Author     :zhuangjf@gmail.com
 Date       : 2014/07/30
 Description:
 History    :
 *******************************************************************************/
#include "process.h"
#include "DB/DBoperation.h"
#include "DB/DBpool.h"
#include "DB/connection_pool.h"
#include "parser.h"
static struct tm tranformTime(std::string &starttime);
void Spilitword(std::string tablename, std::set<std::string> &stopwords);
DBoperation query;
DBpool insert;
Parser parser;
#define SQL_ADDR "192.168.1.105"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"

/**
 *  @brief Init_Main
 *
 *  @param
 *  @return
 */
void Init_Main() {
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root",
      "123456", 50);
  insert.DBinit("use test", connpool);
  query.DBinit(SQL_ADDR, SQL_USER, SQL_PWD, SQL_DATABASE);
  query.DBConnect();
}


/**
 *  @brief FilterTables return the tables to be processed
 *
 *  @param
 *  @return
 */
std::set<std::string>& FilterTables() {
  /// get tables from to be processed
  std::list<std::string> tables;
  query.GetTables(tables);
  /// get tables that have already been processed
  std::list<std::string> inserttables;
  insert.GetTables(inserttables);

  /// remove the duplicated tables
  std::set<std::string> settable;
  for (std::list<std::string>::iterator it_table = tables.begin(), 
       std::list<std::string>::iterator end_table = tables.end();
       it_table != end_table; 
       it_table++) {
    settable.insert(*it_table);
  }
  for (std::list<std::string>::iterator it_inserttable = inserttables.begin(),
       std::list<std::string>::iterator end_inserttable = inserttables.end();
       it_inserttable != end_inserttable; 
       it_inserttable++) {
    if (settable.count(*it_inserttable) == end_inserttable)
      settable.erase(*it_inserttable);
  }
  return settable;
}

#ifdef DEBUG
int main() {
  Init_Main();
  std::set<std::string> tables = FilterTables();
  for (std::string t : tables) {
    Spilitword(*it_table);
  }
  std::cout<<"finish the program"<<std::endl;
  getchar();
  return 1;
}

#endif


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

  }
}
/**
 * 这个是主要处理的 先从数据库里面提取数据，然后分词，最后插入
 */
void Spilitword(std::string tablename, std::set<std::string> &stopwords) {
  std::list<std::list<std::string> > resultList;
  query.SetTableName(tablename);
  insert.SetTableName(tablename);
  //建立数据库的表
  insert.CreateTable();
  long count = query.Getcount();
  std::cout << tablename << " " << count << std::endl;
#ifdef DEBUG
  time_t startT, endT;
  double total;
#endif
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
      parser.LexcicalAnalysis(rawtext, words);
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
    insert.DB_insertData(insert_datas);
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




