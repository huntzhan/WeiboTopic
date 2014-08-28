/**
 * @file    ref.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/26/2014 08:58:59 PM
 * @version 1.0
 *
 */

#ifndef ref_INC
#define ref_INC

#include <map>
#include <string>
#include <vector>
#include "simhash/simhash.h"
using std::to_string;
using std::string;
using std::vector;

/**
 *  @brief RefCount 
 */
class RefCount {
  public:
    // ====================  LIFECYCLE     ==================================
    RefCount() {}
    ~RefCount() {}

    unsigned int AddFingerPrint(const std::vector<string> &instance, int dist) {
      unsigned int pf = sim.BlogHashAfterParser(instance);
      vector<unsigned int> pf_values;
      sim.HammingValuesWithinDistance(pf, dist, pf_values);
      bool flag = false;
      unsigned int res;
      unsigned int max_count = 0;
      for (auto v : pf_values){
        if(ref.find(v) == ref.end())
          continue;
        else {
          flag = true;
          if (ref[v] > max_count){
            max_count = ref[v];
            res = v;
          }
        }
      }
      if (! flag) {
        res = pf;
        ref[pf] = 1u;
      } else {
        ref[res] = ref[res]+1;
      }
      return res;
    }
    unsigned int GetRefCount(const std::vector<string> &instance, int dist, unsigned int &ret_pf) {
      unsigned int pf = sim.BlogHashAfterParser(instance);
      vector<unsigned int> pf_values;
      sim.HammingValuesWithinDistance(pf, dist, pf_values);
      unsigned int max_count= 0;
      for (auto v : pf_values) {
        if (ref.find(v) == ref.end())
          continue;
        else {
          ret_pf = v;
          if (ref[v] > max_count) {
            max_count = ref[v];
            ret_pf = v;
          }
        }
      }
      return max_count;
    }
    void Clear() {
      ref.clear();
    }

  private:
    SimHash sim;
    std::map<unsigned int, unsigned int>ref;

    // DISALLOW_COPY_AND_ASSIGN
    RefCount(const RefCount&);
    void operator=(const RefCount&);
};
#endif
