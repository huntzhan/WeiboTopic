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

    void AddFingerPrint(const std::vector<string> &instance) {
      unsigned int pf = sim.BlogHashAfterParser(instance);
      if(ref.find(pf) == ref.end())
        ref[pf] = 1u;
      else
        ref[pf] = ref[pf]+1;
    }
    unsigned int GetRefCount(const std::vector<string> &instance) {
      unsigned int pf = sim.BlogHashAfterParser(instance);
      return ref[pf];
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
