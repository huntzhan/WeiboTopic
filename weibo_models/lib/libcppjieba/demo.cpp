#include "KeywordExtractor.hpp"
using namespace CppJieba;

int main(int argc, char ** argv)
{
    KeywordExtractor extractor("dict/jieba.dict.utf8", "dict/hmm_model.utf8", "dict/idf.utf8", "dict/stop_words.utf8");
    string s("产妇命丧手术台 医生护士齐失踪");
    vector<pair<string, double> > wordweights;
    vector<string> words;
    size_t topN = 5;
    extractor.extract(s, wordweights, topN);
    cout<< s << '\n' << wordweights << endl;
    extractor.extract(s, words, topN);
    cout<< s << '\n' << words << endl;
    return EXIT_SUCCESS;
}
