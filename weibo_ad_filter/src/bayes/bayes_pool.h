/**
 * @file    bayes_pool.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/31/2014 03:27:55 PM
 * @version 1.0
 *
 */

#ifndef bayes_pool_INC
#define bayes_pool_INC

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
 *  @brief BayesPool 
 */
class BayesPool {
  public:
    std::list<shared_ptr<Bayes>> bayes_pool_() {
      return bayes_pool;
    }
    void AddBayesClassifier(shared_ptr<Bayes> bayes) {
      for (auto bayes : bayes_pool) {
        ;
      }
    }

    // ====================  LIFECYCLE     ==================================
    BayesPool();
    ~BayesPool();

  private:
    std::list<shared_ptr<Bayes>> bayes_pool;

    // DISALLOW_COPY_AND_ASSIGN
    BayesPool(const BayesPool&);
    void operator=(const BayesPool&);
};

/**
 *  @brief Constructor
 *
 *  @param
 */
BayesPool::BayesPool() {
}

/**
 *  @brief Destructor
 *
 *  @param
 */
BayesPool::~BayesPool() {
}

#endif
