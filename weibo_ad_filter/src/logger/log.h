/**
 * @file    log.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/26/2014 11:42:25 AM
 * @version 1.0
 *
 */


#ifndef log_INC
#define log_INC
#include <fstream>
#include <string>
#include <map>
#include <memory>
using std::ofstream;
using std::endl;
using std::string;
using std::shared_ptr;

enum LogType {
  BEGIN_T,
  ZOMBIE_T,
  REFLOW_T,
  REFHIGH_T,
  END_T
};

/**
 *  @brief Log 
 */
class Log {
  public:
    static const string LOG_FILES[END_T];
    static void Logging(LogType type, const string &record);

  private:
    static void _InitialLoggers();
    static shared_ptr<ofstream> _GetStreamByType(LogType type);

    static std::map<LogType, shared_ptr<ofstream>> _loggers;

    // DISALLOW_COPY_AND_ASSIGN
    Log() {}
    ~Log() {}
    Log(const Log&);
    void operator=(const Log&);
};
#endif
