/**
 * @file    log.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/26/2014 11:43:08 AM
 * @version 1.0
 *
 */

#include <ctime>
#include <cstdlib>
#include <utility>
#include "log.h"

std::map<LogType, shared_ptr<ofstream>> Log::_loggers;
const string Log::LOG_FILES[END_T] = {
  "",
  "run.log",
  "zombie.log",
  "zombie_sim.log",
  "ref_dist_1.log",
  "tactic.log",
  "tactic_source.log"
};
/**
 *  @brief _InitialLoggers
 *
 *  @param
 */
void Log::_InitialLoggers() {
  srand(time(nullptr));
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

/**
 *  @brief LoggingRandom only write a record every random_size times
 *
 *  @param random_size has to be less then RAND_MAX
 */
void Log::LoggingRandom(LogType type, const unsigned int random_size, const string &record) {
  int r = rand();
  if(r % random_size == 0)
    Logging(type, record);
}
