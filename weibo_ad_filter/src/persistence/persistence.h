/**
 * @file    persistence.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/27/2014 10:33:55 PM
 * @version 1.0
 *
 */


#ifndef persistence_INC
#define persistence_INC
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "logger/log.h"
#include "db/model.h"
#include "database/mysql_handler.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

DBpool insert;

/**
 *  @brief Persistence 
 */
class Persistence {
  public:
    // ====================  LIFECYCLE     ==================================
    Persistence();
    ~Persistence();
    void Insert(const string &tablename, );
    void Insert(const string &tablename, const std::vector<INSERT_DATA> &insert_datas) {
      InsertDataToTable(tablename, insert_datas);
    }

  private:
    std::shared_ptr<mysql_handler::FilteredBlogHandler> handler;

    // DISALLOW_COPY_AND_ASSIGN
    Persistence(const Persistence&);
    void operator=(const Persistence&);
};

/**
 *  @brief Constructor
 *
 *  @param
 */
Persistence::Persistence() {
  ConnPool *connpool = ConnPool::GetInstance("tcp://127.0.0.1:3306", "root", "123456", 50);
  insert.DBinit("use filter", connpool);
  handler = std::make_shared<mysql_handler::SpamHandler>("split", "");
  handler->Init(); 
}

/**
 *  @brief Destructor
 *
 *  @param
 */
Persistence::~Persistence() {
}

#endif
