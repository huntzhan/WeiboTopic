/**
 * @file    preprocessor.cpp
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 10:16:45
 * @version 1.0
 *
 * 
 */

#include "preprocessor.h"
/**
 *  @brief PerformTactic return true if the blog is ok
 *  @param
 *  @return
 */
bool Preprocessor::PerformTactic(const Blog& b){
  if (t_zombie.IsSpam(b)){
    AddFingerPrint(b, 1);
    return false;
  }
  else if (IsBlogInFingerprints(b, 1)){  /// valid user, test it's blog
    cout<<"#####"<<endl<<b.m_content<<endl;
    return false;
  }
  else return true;
  // if (t_topic.IsSpam(b))
  //   return false;
  // if (t_user.IsSpam(b))
  //   return false;
  return true;
}

/**
 *  @brief AddFingerPrint
 *  @param
 *  @return
 */
void Preprocessor::AddFingerPrint(const Blog &b, int dist) {
  /// blog is determined as spam, add fingerprint of it
  unsigned int v = sim.BlogHash(b.m_content.c_str());
  vector<unsigned int> all_values;
  sim.HammingValuesWithinDistance(v, dist, all_values);
  bool is_in = false;
  for(const auto &i : all_values){
    if(fingerprint.find(i) != fingerprint.end()){
      is_in = true;
      break;
    }
  }
  if (!is_in){
    fingerprint.insert(v);
    if(fingerprint.size() == FLUSH_DB_THRED)
      FlushCachedFingerprint(dist);
  }
}

/**
 *  @brief flush and clear the cache
 *  first query db for duplicated simhash values within dist, then flush non-duplicated values at one time
 *  @param
 *  @return
 */
void Preprocessor::FlushCachedFingerprint(int dist) {
  vector<unsigned int> value_to_insert;
  for(const auto &i : fingerprint){
    vector<unsigned int> all_value;
    sim.HammingValuesWithinDistance(i, dist, all_value);
    if (IsSimhashValuesInDB(all_value))
      continue;
    value_to_insert.push_back(i);
  }
  Flush(value_to_insert);
  value_to_insert.clear();
}

/**
 *  @brief Test whether blog's simhash value has been recorded
 *  @param
 *  @return true if simhash has been recorded
 */
bool Preprocessor::IsBlogInFingerprints(const Blog &b, int dist) {
	unsigned int v = sim.BlogHash(b.m_content.c_str());
    vector<unsigned int> all_value;
    sim.HammingValuesWithinDistance(v, dist, all_value);
    /// test whether in cache
    for(const auto &i : all_value){
      if(fingerprint.find(i) != fingerprint.end())
        return true;
    }
    /// test whether in db
    if(IsSimhashValuesInDB(all_value))
        return true;
    return false;
}

/**
 *  @brief Flush flush the simhash value in set into db 
 *  @param
 *  @return 
 */
void Preprocessor::Flush(vector<unsigned int> &v) {
  size_t count = handler->AddSpams(v);
  if(count != v.size()) {
    cout<< "SpamHandler AddSpams inserts " << v.size() 
      << " with " << count << "successfully inserted." << endl;
  }
} 

/**
 *  @brief IsSimhashValuesInDB
 *  @param
 *  @return 
 */
bool Preprocessor::IsSimhashValuesInDB(vector<unsigned int> v) {
  return handler->QuerySpamSimhash(v);
}
