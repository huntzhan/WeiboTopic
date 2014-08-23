#ifndef _SIMHASH_H
#define _SIMHASH_H

#include <vector>
#include <string>
#include "split/parser.h"
using std::vector;
using std::string;

class SimHash {
  public:
    unsigned int BlogHash(const char *m_content);
    unsigned int BlogHashAfterParser(const std::vector<std::string> &words);
    int Calculate_Distance(unsigned int hashvalue1, unsigned int hashvalue2);
    void HammingValuesWithinDistance(unsigned int value, int dist, vector<unsigned int> &res);

  private:
    unsigned int RSHash(const char *str);
    void _HammingValuesWithinDistance(unsigned int value, int after, int dist, vector<unsigned int> &res);

    Parser parser;
    static const int BITS = 32;
    unsigned int xor_arr[BITS] = {
      1u,
      2u,
      4u,
      8u,
      16u,
      32u,
      64u,
      128u,
      256u,
      512u,
      1024u,
      2048u,
      4096u,
      8192u,
      16384u,
      32768u,
      65536u,
      131072u,
      262144u,
      524288u,
      1048576u,
      2097152u,
      4194304u,
      8388608u,
      16777216u,
      33554432u,
      67108864u,
      134217728u,
      268435456u,
      536870912u,
      1073741824u,
      2147483648u
    };
};

#endif
