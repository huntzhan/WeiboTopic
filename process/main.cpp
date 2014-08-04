#include "process.h"
#include <iostream>
int main()
{
	std::set<std::string> stopwords;
	MakeStopSet(stopwords);
	std::string ID="3566050291450093";
	Get_StringVector(ID,stopwords);
	return 1;
}	

