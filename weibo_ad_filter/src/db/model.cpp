/**
 * @file    model.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 09:49:09 PM
 * @version 1.0
 *
 */

#include "model.h"
#include <iostream>
#include <sstream>
using std::cout;
using std::endl;

void PrintBlog(const Blog &b) {
  cout<<"User: "
    <<b.u_uid
    <<" "<<b.sex
    <<" "<<b.u_created_at
    <<" "<<b.u_followees
    <<" "<<b.u_fans
    <<" "<<b.u_bi_followers_count
    <<" "<<b.u_posts
    <<" "<<b.u_favourites_count
    <<" "<<b.u_vierfied;
  cout<<endl;
  cout<<"Blog: "
    <<b.m_mid
    <<" "<<b.m_created_time
    <<"||| "<<b.m_content
    <<" ||| "<<b.m_source
    <<" "<<b.m_favorites
    <<" "<<b.m_comments
    <<" "<<b.m_forwards;
  cout<<endl<<"###########################"<<endl;
}

string Blog2Str(const Blog &b) {
  std::ostringstream ss;
  ss<<"User: "
    <<b.u_uid
    <<" "<<b.sex
    <<" "<<b.u_created_at
    <<" ["<<b.u_followees
    <<" "<<b.u_fans
    <<" "<<b.u_posts
    <<"] "<<b.u_bi_followers_count
    <<" "<<b.u_favourites_count
    <<" "<<b.u_vierfied;
  ss<<" ";
  ss<<"Blog: "
    <<b.m_mid
    <<" "<<b.m_created_time
    <<" ["<<b.m_content<<"]"
    <<" "<<b.m_source
    <<" "<<b.m_favorites
    <<" "<<b.m_comments
    <<" "<<b.m_forwards;
  return ss.str();
}
