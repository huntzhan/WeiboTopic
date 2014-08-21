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
  std::vector<unsigned int> hashword;
  parser.LexicalAnalysisWord(m_content, word);
  for (std::vector<std::string>::iterator it_word = word.begin(), end_word =
      word.end(); it_word != end_word; it_word++) {
    ///计算一个单词的hash值
    unsigned int res = RSHash((*it_word).c_str());
    hashword.push_back(res);
  }
  unsigned int hashvalue = 0;
  for (int i = 0; i < 32; i++) {
    int sum = 0;
    ///所有特征向量进行加权（1则为正，0则为负），然后累加 这里没有用到权值
    for (std::vector<unsigned int>::iterator it_hash = hashword.begin(),
        end_hash = hashword.end(); it_hash != end_hash; it_hash++) {
      if (((0x00000001 << i) & (*it_hash))) {
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
