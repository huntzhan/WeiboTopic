/**
 * @file    fuzzy.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/29/2014 10:42:54 AM
 * @version 1.0
 *
 */


#ifndef fuzzy_INC
#define fuzzy_INC
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
using std::string;
using std::cout;
using std::endl;
using std::vector;

/**
 *  @brief Fuzzy 
 */
class Fuzzy {
  public:
    // ====================  LIFECYCLE     ==================================
    Fuzzy();
    explicit Fuzzy();
    ~Fuzzy();

  private:

    // DISALLOW_COPY_AND_ASSIGN
    Fuzzy(const Fuzzy&);
    void operator=(const Fuzzy&);
};

/**
 *  @brief Constructor
 *
 *  @param
 */
Fuzzy::Fuzzy() {
}

/**
 *  @brief Destructor
 *
 *  @param
 */
Fuzzy::~Fuzzy() {
}

#endif
