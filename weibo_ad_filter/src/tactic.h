/**
 * @file    tactic.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 06:48:08 PM
 * @version 1.0
 *
 * @section LICENSE
 * 
 */

#ifndef tactic_INC
#define tactic_INC
#include "DB/model.h"
/**
 *  @brief Tactic Base class for different tactic performed on every single message
 */
class Tactic {
public:
  // ====================  LIFECYCLE     ==================================
  Tactic() {}
  ~Tactic() {}
  virtual bool IsSpam(Blog b) = 0;

private:

  // DISALLOW_COPY_AND_ASSIGN
  Tactic(const Tactic&);
  void operator=(const Tactic&);
};


/**
 *  @brief ZombieTactic 
 */
class ZombieTactic : public Tactic {
  public:
    // ====================  LIFECYCLE     ==================================
    ZombieTactic() {}
    ~ZombieTactic() {}
    virtual bool IsSpam(Blog b);

  private:

    // DISALLOW_COPY_AND_ASSIGN
    ZombieTactic(const ZombieTactic&);
    void operator=(const ZombieTactic&);
};
#endif
