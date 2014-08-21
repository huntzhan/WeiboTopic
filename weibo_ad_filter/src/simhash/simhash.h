#ifndef _SIMHASH_H
#define _SIMHASH_H

#include "split/parser.h"

class SimHash {
public:
	unsigned int RSHash(const char *str);
	unsigned int BlogHash(const char *m_content);
	int Calculate_Distance(unsigned int hashvalue1, unsigned int hashvalue2);
private:
	Parser parser;
};

#endif
