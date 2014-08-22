/**
 * @file    preprocessor.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 10:16:45
 * @version 1.0
 *
 * @section LICENSE
 *
 */

#ifndef preprocessor_INC
#define preprocessor_INC

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include "db/model.h"
#include "tactic.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::list;
/**
 *  @brief Preprocessor return true if the blog is useful
 */
class Preprocessor {
  public:
    // ====================  LIFECYCLE     ==================================
    Preprocessor() {}
    ~Preprocessor() {}
    bool PerformTactic(const Blog& b){
      if (t_zombie.IsSpam(b))
        return false;
   //  if (t_topic.IsSpam(b))
  //	  return false;
     if (t_user.IsSpam(b))
         	  return false;
      return true;
    }

  private:
    ZombieTactic t_zombie;
    TopicTcatic t_topic;
    UserTactic t_user;
    // DISALLOW_COPY_AND_ASSIGN
    Preprocessor(const Preprocessor&);
    void operator=(const Preprocessor&);
};
#endif
