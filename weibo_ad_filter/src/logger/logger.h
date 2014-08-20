/**
 * @file    logger.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/20/2014 12:02:27 PM
 * @version 1.0
 *
 * @section LICENSE
 */

#include <fstream>
#include <string>
#include <list>
#include "db/model.h"
using std::ofstream;
using std::ifstream;
using std::endl;
using std::string;
using std::list;

#define SQL_ADDR "192.168.1.108"
#define SQL_USER "root"
#define SQL_PWD    "123456"
#define SQL_DATABASE "sina"

/**
 *  @brief Logger 
 */
class Logger {
  public:
    // ====================  LIFECYCLE     ==================================
    Logger(string path);
    ~Logger();
    bool HasNextBlog();
    Blog NextBlog();
    /// void AddRemovedBlog(const Blog &b)

  private:
    list<Blog> cached_blogs;

    ofstream out;
    const static int ROW_EACH_TIME = 1000;
    const string tablename = "Microblog1408215600";
    int number_all_rows = 0;
    int number_left_rows = 0;
    /// int number_removed_rows = 0;
    /// int number_spam_rows = 0;

    // DISALLOW_COPY_AND_ASSIGN
    Logger(const Logger&);
    void operator=(const Logger&);
};

