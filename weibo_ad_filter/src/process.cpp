/******************************************************************************* 
 Filename   : process.cpp
 Version    : ver 1.0
 Author     :zhuangjf@gmail.com
 Date       : 2014/07/30
 Description:
 History    :
 *******************************************************************************/

void display(std::list<std::list<std::string> > &msg) ;
void count_the_user(list<Blog> &weibos);
void FilterTables(std::set<std::string> &settable);
INSERT_DATA PackInsertData(const Blog &b, const vector<Word> &words);
void FilterOneTable(string table, vector<INSERT_DATA> &insert_datas);
void InsertDataToTable(std::string tablename, std::vector<INSERT_DATA> &insert_datas);
INSERT_DATA PackInsertData(const Blog &b, const vector<Word> &words);
bool SortCmp(const std::pair<string,int> &key1 ,const std::pair<string ,int> &key2 ){
  if (key1.second > key2.second)
    return true;
  return false;
}
/*
 *计算1个小时内重复发微博的数目
 *
 */
void count_the_user(list<Blog> &weibos) {
  list<string> badweibolist;
  SimHash simhash;
  std::map<string, int> users;
  std::cout << weibos.size() << endl;
  for (std::list<Blog>::iterator ib = weibos.begin(), ie = weibos.end();
      ib != ie; ib++) {
    std::list<string> m_list;
    std::pair<std::map<std::string, int>::iterator, bool> ret =
        users.insert(std::make_pair((*ib).u_uid, 1));
    if (!ret.second) {
      ++ret.first->second;  /// inc the user count
    }

  }
  std::map<string, int>::iterator it_map = users.begin();
  std::map<string, int>::iterator end_map = users.end();
  std::vector<std::pair<string, int> > vector_sort;
  for (; it_map != end_map; it_map++) {
    vector_sort.push_back(make_pair(it_map->first, it_map->second));
  }
    sort(vector_sort.begin(),vector_sort.end(),SortCmp);
  ///获得前100的用户重复
  std::string badUID = " ";
  unsigned countsum=vector_sort.size()>1000?1000:vector_sort.size();
  std::cout<<countsum<<std::endl;
  for (int i_count = 0; i_count < countsum;i_count++) {
         if(vector_sort[i_count].second<3){
           break;
         }
    std::cout << "uid----" << vector_sort[i_count].first << "----num---"
        << vector_sort[i_count].second << std::endl;
    vector<pair<unsigned int, std::string>> hashvector;
    vector<std::string> mids;
    for (std::list<Blog>::iterator ib = weibos.begin(), ie = weibos.end();
        ib != ie; ib++) {
      if ((*ib).u_uid == vector_sort[i_count].first) {
        hashvector.push_back(
            make_pair(simhash.BlogHash(ib->m_content.c_str()),ib->m_content));
                mids.push_back(ib->m_mid);
        // std::cout<<ib->u_fans<<"  "<<ib->u_followees<<ib->m_content<<std::endl;
      }
      if ((*ib).u_uid == badUID) {
        ///删除坏的用户的微博
        std::cout<<badUID<<std::endl;
        std::cout<<ib->m_content<<std::endl;
        ib=weibos.erase(ib);
      }

    }
    badUID = vector_sort[i_count].first;
    vector<int> hashresult;
    int count = 0;
    //计算用户在一个小时内所发的微博的两两hash值
    for (unsigned int i_first = 0; i_first < hashvector.size(); i_first++) {
      for (unsigned int j_second = i_first; j_second < hashvector.size(); j_second++) {
        hashresult.push_back(
            simhash.Calculate_Distance(hashvector[i_first].first,
                hashvector[j_second].first));
        count++;
      }
    }
    int sum = 0;
    for (unsigned int i = 0; i < hashresult.size(); i++) {
      sum += hashresult[i];
    }
    ///如果hash的平均值小于3，就是说这个小时内所发的微博内容几乎一样
    if (count != 0) {
      if (double((double) sum / count) < 3) {
        for (unsigned int i = 0; i < hashvector.size(); i++) {
          std::cout << hashvector[i].second << std::endl;
          badweibolist.push_back(mids[i]);
          ///把改ID记录下来 下次循环的时候就删除该用户所发的微博
        }
      }
      else{
        badUID=" ";
      }
    }
      std::cout << "-------------------" << double((double) sum / count)<< "---------------------------" << std::endl;
//    std::cout << "################################################"
//        << std::endl;
  }

}

/**
 *  @brief FilterTables return the tables to be processed
 *
 *  @param
 *  @return
 */
 void FilterTables(std::set<std::string> &settable) {
  /// get tables from to be processed
  std::list<std::string> tables;
  query.GetTables(tables);
  /// get tables that have already been processed
  std::list<std::string> inserttables;
  insert.GetTables(inserttables);

  /// remove the duplicated tables
  std::list<std::string>::iterator it_table = tables.begin();
  std::list<std::string>::iterator end_table = tables.end();
  for ( ; it_table != end_table; it_table++) {
    settable.insert(*it_table);
  }
  std::list<std::string>::iterator it_inserttable = inserttables.begin();
  std::list<std::string>::iterator end_inserttable = inserttables.end();
  for ( ; it_inserttable != end_inserttable; it_inserttable++) {
    if (settable.count(*it_inserttable))
      settable.erase(*it_inserttable);
  }
  std::cout<<settable.size()<<std::endl;
}

