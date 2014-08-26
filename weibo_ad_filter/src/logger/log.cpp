/**
 * @file    log.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/26/2014 11:43:08 AM
 * @version 1.0
 *
 */

#include <ctime>
#include <utility>
#include "log.h"

std::map<LogType, shared_ptr<ofstream>> Log::_loggers;
const string Log::LOG_FILES[END_T] = {
  "",
  "zombie.log",
  "ref_low.log",
  "ref_high.log"
};
/**
 *  @brief _InitialLoggers
 *
 *  @param
 */
void Log::_InitialLoggers() {
  for (int i = BEGIN_T+1; i!= END_T; i++) {
    shared_ptr<ofstream> pf(new ofstream(LOG_FILES[i], std::fstream::app));
    Log::_loggers.insert(std::pair<LogType, shared_ptr<ofstream>>(static_cast<LogType>(i), pf));
  }
}

/**
 *  @brief _GetStreamByType
 *
 *  @param
 */
shared_ptr<ofstream> Log::_GetStreamByType(LogType type) {
  if (Log::_loggers.size() == 0) {
    Log::_InitialLoggers();
  }
  return Log::_loggers[type];
}

/**
 *  @brief Logging
 *
 *  @param
 */
void Log::Logging(LogType type, const string &record) {
  shared_ptr<ofstream> out = _GetStreamByType(type);

  auto t = std::time(nullptr);
  auto tm = std::localtime(&t);
  char buffer[80];
  strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", tm);
  string time_str(buffer);
  *out << "<" << time_str << "> " << record << endl;
}
