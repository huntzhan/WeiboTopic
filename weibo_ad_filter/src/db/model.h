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
#include <vector>
#include <set>
#include <iostream>
#include <sstream>
using std::string;

typedef struct _Word {
  std::string word;
  std::string proper;
} Word;

typedef struct _insert_data {
  std::string mid;
  std::string text;
  std::string spilt;
} INSERT_DATA;

typedef struct _BLOG {
  string m_mid;
  string m_created_time;
  string m_content;
  string m_source;
  long m_favorites;
  long m_comments;
  long m_forwards;

  string u_uid;
  string sex;
  string u_created_at;
  long u_followees;
  long u_fans;
  long u_bi_followers_count;
  long u_posts;
  long u_favourites_count;
  long u_vierfied;
} Blog;

void PrintBlog(const Blog &b);
string Blog2Str(const Blog &b);
#endif

