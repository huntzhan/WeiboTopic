#include "KeywordExtractor.hpp"
using namespace CppJieba;

int main(int argc, char ** argv)
{
    KeywordExtractor extractor("data/jieba/jieba.dict.utf8", "data/jieba/hmm_model.utf8", "data/jieba/idf.utf8", "data/jieba/stop_words.utf8");
    string s("产妇命丧手术台医生护士齐失踪");
    vector<pair<string, double> > wordweights;
    vector<string> words;
    size_t topN = 10;
    extractor.extract(s, wordweights, topN);
    cout<< s << '\n' << wordweights << endl;
    extractor.extract(s, words, topN);
    cout<< s << '\n' << words << endl;
    return EXIT_SUCCESS;
}
