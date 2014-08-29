/**
 * @file    keyword.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/29/2014 05:13:20 PM
 * @version 1.0
 *
 */

#ifndef keyword_INC
#define keyword_INC
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <utility>
using std::string;
using std::vector;

/**
 *  @brief KeyWord 
 */
class KeyWord {
  public:
    // ====================  LIFECYCLE     ==================================
    explicit KeyWord(string w, unsigned total): keyword(w), total_count(total) {}
    ~KeyWord() {}
    void AddCatalogProb(const unsigned hash_val, const double p) {
      catalog_to_prob.insert(std::pair<unsigned, double>(hash_val, p));
    }
    void set_prob(const double p) {
      prob = p;
    }
    void IncCount() {
      count_in_total++;
      prob = count_in_total*1.0 / total_count;
    }
    vector<unsigned> GetCatalogs() {
      vector<unsigned> v;
      for (auto &pir : catalog_to_prob) {
        v.push_back(pir.first);
      }
      return v;
    }
    double GetProbOfCatalog(const unsigned hash_val) {
      return catalog_to_prob[hash_val];
    }

  private:
    std::map<unsigned, double> catalog_to_prob;
    string keyword;
    unsigned count_in_total;
    unsigned total_count = 0;
    double prob;

    // DISALLOW_COPY_AND_ASSIGN
    KeyWord(const KeyWord&);
    void operator=(const KeyWord&);
};
#endif
