#ifndef COOCCURRENCEWORD_H_
#define COOCCURRENCEWORD_H_
#include<iostream>
#include<map>
#include<string>
class CooccurrenceWord{
public:
	std::map<std::string,double>word_cooccurrence;
	std::map<std::string,double> * GetWordCooccurrence(){
		return & word_cooccurrence;
	}
};
#endif
