/**
 * @file    tactic.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 06:50:44 PM
 * @version 1.0
 * 
 */

#include "tactic.h"
/**
 *  @brief IsSpam examine the user based on their number of fans, followees, bi-follow, and posts
 *
 *  @param
 *  @return
 */
bool ZombieTactic::IsSpam(Blog b) {
  if (b.u_vierfied)
    return false;
  if (b.u_bi_followers_count*1.0 / b.u_followees < 0.2)
    return true;
}
