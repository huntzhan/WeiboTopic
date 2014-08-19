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
  long followees;
  long fans;
  long bi_followers_count;
  long posts;
  string sex;
  long favourites_count;
  string created_at;
  long verified;
};

struct Blog{
  string mid;
  string uid; /// added for foreign relationship, not exists in table
  string created_time;
  string content;
  long favorites;
  long comments;
  long forwards;
  string source;
}
#endif

