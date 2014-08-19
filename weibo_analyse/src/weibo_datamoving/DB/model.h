/**
 * @file    model.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 10:16:45
 * @version 1.0
 *
 * ORM mapping from database tables to C++ structures
 */

#ifndef model_INC
#define model_INC
#include <string>
using std::string;

struct User{
  string uid;
};

struct Blog{
  /// user data
  string u_uid;
  long u_followees;
  long u_fans;
  long u_bi_followers_count;
  long u_posts;
  string u_sex;
  long u_favourites_count;
  string u_created_at;
  long u_verified;
  /// blog data
  string m_mid;
  string m_created_time;
  string m_content;
  long m_favorites;
  long m_comments;
  long m_forwards;
  string m_source;
}
#endif

