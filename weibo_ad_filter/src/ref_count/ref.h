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

    /**
     * @brief inc the ref count of fp or it's variants, and return the detected one in map
     */
    unsigned int AddFingerPrint(const unsigned fp, int dist) {
      vector<unsigned int> fp_values;
      sim.HammingValuesWithinDistance(fp, dist, fp_values);
      bool flag = false;
      unsigned int res;
      unsigned int max_count = 0;
      for (auto v : fp_values){
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
        res = fp;
        ref[fp] = 1u;
      } else {
        ref[res] = ref[res]+1;
      }
      return res;
    }

    unsigned int AddFingerPrint(const std::vector<string> &instance, int dist) {
      unsigned int fp = sim.BlogHashAfterParser(instance);
      return AddFingerPrint(fp, dist);
    }

    unsigned int GetRefCount(const unsigned fp, int dist, unsigned &ret_fp) {
      vector<unsigned int> fp_values;
      sim.HammingValuesWithinDistance(fp, dist, fp_values);
      unsigned int max_count= 0;
      for (auto v : fp_values) {
        if (ref.find(v) == ref.end())
          continue;
        else {
          ret_fp = v;
          if (ref[v] > max_count) {
            max_count = ref[v];
            ret_fp = v;
          }
        }
      }
      return max_count;
    }

    unsigned int GetRefCount(const std::vector<string> &instance, int dist, unsigned int &ret_fp) {
      unsigned int fp = sim.BlogHashAfterParser(instance);
      return GetRefCount(fp, dist, ret_fp);
    }

    unsigned GetRefSize() {
      return ref.size();
    }

    void Clear() {
      ref.clear();
    }

  private:
    static SimHash sim;
    std::map<unsigned int, unsigned int>ref;

    // DISALLOW_COPY_AND_ASSIGN
    RefCount(const RefCount&);
    void operator=(const RefCount&);
};

SimHash RefCount::sim;
#endif
