#include "simhash.h"
/**  计算一个词语的hash值
 *   最简单的RShash计算方法
 */
unsigned int SimHash::RSHash(const char *str) {
  unsigned int b = 378551;
  unsigned int a = 63689;
  unsigned int hash = 0;
  while (*str) {
    hash = hash * a + (*str++);
    a *= b;
  }
  return (hash & 0x7FFFFFFF);
}

/*
 * 计算一条微博的hash值  因为是短文本没有加权
 *
 */
unsigned int SimHash::BlogHash(const char *m_content) {
  std::vector<std::string> word;
  parser.LexicalAnalysisWord(m_content, word);
  return BlogHashAfterParser(word);
}

/*
 * 计算一条分词后的微博的hash值  因为是短文本没有加权
 *
 */
unsigned int SimHash::BlogHashAfterParser(const std::vector<std::string> &words) {
  std::vector<unsigned int> hashword;
  for(const string &word : words) {
    ///计算一个单词的hash值
    unsigned int res = RSHash((word).c_str());
    hashword.push_back(res);
  }
  unsigned int hashvalue = 0;
  for (int i = 0; i < 32; i++) {
    int sum = 0;
    ///所有特征向量进行加权（1则为正，0则为负），然后累加 这里没有用到权值
    for(unsigned int v : hashword) {
      if (((0x00000001 << i) & (v))) {
        sum++;
      } else {
        sum--;
      }
    }
    ///对于n-bit签名的累加结果，如果>0置1，否则置0
    if (sum > 0) {
      hashvalue |= 0x00000001 << i;
    } else {
      hashvalue |= 0x00000000 << i;
    }
  }
  return hashvalue;
}

/**
 * 计算海明距离
 */
int SimHash::Calculate_Distance(unsigned int hashvalue1,
    unsigned int hashvalue2) {
  int same = 0;
  unsigned int bitvalue = hashvalue1 ^ hashvalue2;
  while (bitvalue) {
    ++same;
    bitvalue &= bitvalue - 1;
  }
  return same;
}

/**
 *  @brief HammingValuesWithinDistance return all values whose distance with v is less or equal to dist
 *
 *  @param
 *  @return
 */
void SimHash::HammingValuesWithinDistance(unsigned int value, int dist, vector<unsigned int> &res) {
  _HammingValuesWithinDistance(value, 0, dist, res);
  res.push_back(value);
}

void SimHash::_HammingValuesWithinDistance(unsigned int value, 
                                           int after,
                                           int dist,
                                           vector<unsigned int> &res) {
  if(dist == 0)
    return;
  for(int i = after; i<SimHash::BITS; i++){
    unsigned int v = value ^ xor_arr[i];
    res.push_back(v);
    _HammingValuesWithinDistance(v, i+1, dist-1, res);
  }
}
