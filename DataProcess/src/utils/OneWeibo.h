

#ifndef ORIGINALWORD_H_
#define ORIGINALWORD_H_

#include<list>
#include<string>

typedef struct _Word{
	std::string word;
	std::string proper;
	bool is_tag_word;
}Word;

class OneWeibo{
public:
	std::string belong_table;
	std::string MID;
	std::list<Word> words;
	std::list<Word> * GetweiboWords(){
		return &this->words;
	}
};
#endif /* ORIGINALWORD_H_ */
