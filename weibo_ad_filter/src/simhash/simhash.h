#ifndef _SIMHASH_H
#define _SIMHASH_H

#include <vector>
#include "split/parser.h"
using std::vector;

class SimHash {
  public:
    unsigned int BlogHash(const char *m_content);
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
      3u,
      4u,
      5u,
      6u,
      7u,
      8u,
      9u,
      10u,
      11u,
      12u,
      13u,
      14u,
      15u,
      16u,
      17u,
      18u,
      19u,
      20u,
      21u,
      22u,
      23u,
      24u,
      25u,
      26u,
      27u,
      28u,
      29u,
      30u,
      31u,
      32u
    };
};

#endif
