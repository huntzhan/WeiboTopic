#include "process.h"
#include <iostream>
int main()
{
	std::set<std::string> stopwords;
	std::vector<std::string> vec;
	MakeStopSet(stopwords);
	std::string ID="3566051122059043";
	Get_StringVector(ID,stopwords,vec);

	for(std::vector<std::string>::iterator it=vec.begin() ; it!=vec.end();it++)
		std::cout<<*it<<std::endl;

	return 1;
}	

