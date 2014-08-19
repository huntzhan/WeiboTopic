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

typedef struct _BLOG {
  string mid;
  string uid; /// added for foreign relationship, not exists in table
  string created_time;
  string content;
  long favorites;
  long comments;
  long forwards;

  long u_followees;
  long u_fans;
  long u_posts;
  long u_favourites_count;
  long u_vierfied;
  string u_created_at;
}Blog;
#endif

