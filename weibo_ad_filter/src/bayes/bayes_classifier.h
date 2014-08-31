/**
 * @file    bayes_classifier.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/29/2014 08:05:11 PM
 * @version 1.0
 *
 */

#ifndef bayes_classifier_INC
#define bayes_classifier_INC
#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "logger/log.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

/**
 *  @brief Bayes 
 */
class Bayes {
  public:
    // ====================  LIFECYCLE     ==================================
    explicit Bayes(const std::list<shared_ptr<ParsedBlog>> &cata_blogs, 
                   const unsigned all_blogs_size,
                   shared_ptr<std::map<string, double>> word_dict,
                   const double threshold) : THRESHOLD(threshold);
    ~Bayes() {}
    bool IsSpam(ParsedBlog &blog, double &res_prob);
    std::map<string, double> prob_cata_given_w_() { return prob_cata_given_w; }
    unsigned GetSizeOfKeyWords() { return prob_cata_given_w.size(); }

  private:
    string _GetSpamWords() {
      std::ostringstream ss;
      for (auto &kv : prob_cata_given_w) {
        ss<<"[" << kv.first << "," << kv.second << "]";
      }
      return ss.str();
    }

    const double THRESHOLD;
    const double SPAM_CATA_GIVEN_WORD_LOWER_BOUND_RATIO = 0.5;
    unsigned cata_size;
    double prob_S;
    std::map<string, double> prob_cata_given_w;

    // DISALLOW_COPY_AND_ASSIGN
    Bayes(const Bayes&);
    void operator=(const Bayes&);
};


explicit Bayes::Bayes(const std::list<shared_ptr<ParsedBlog>> &cata_blogs, 
    const unsigned all_blogs_size,
    shared_ptr<std::map<string, double>> word_dict,
    const double threshold) : THRESHOLD(threshold) { 
  cata_size = cata_blogs.size();
  prob_S = cata_size*1.0 / all_blogs_size;
  std::map<string, unsigned> counter;
  for (auto &blog : cata_blogs) {
    for (auto &w : blog->Towords()) {
      /// p(w|S)
      if (counter.find(w) == counter.end())
        counter[w] = 1;
      else 
        counter[w] += 1;
    }
  }
  for (auto &kv : counter) {
    string w = kv.first;
    double prob_w_given_S_ = kv.second*1.0 / cata_size;
    double prob_w_given_S = prob_w_given_S_>1.0? 1.0 : prob_w_given_S_;
    double prob_w = (*word_dict)[w];
    double prob_cata_given_w_ = prob_w_given_S*prob_S / prob_w;
    if (prob_cata_given_w_ > SPAM_CATA_GIVEN_WORD_LOWER_BOUND_RATIO)
      prob_cata_given_w[w] = prob_cata_given_w_;
  }
  Log::Logging(BAYES_T, "Bayes: " + _GetSpamWords() + ">" + std::to_string(prob_S) + ">" + std::to_string(cata_size));
}

bool Bayes::IsSpam(ParsedBlog &blog, double &res_prob) {
  double prob_numerator = 1,
         prob_denominator = 1;
  bool is_tested = false;
  unsigned match_word_count = 0;
  std::set<string> dupicate_word;
  /// test
  std::ostringstream ss;
  for (auto &w : blog.Towords()) {
    if (prob_cata_given_w.find(w) == prob_cata_given_w.end())
      continue;
    is_tested = true;
    if (dupicate_word.find(w) == dupicate_word.end())
      dupicate_word.insert(w);
    else continue;
    match_word_count++;
    double prob = prob_cata_given_w[w];
    ss<< "[" << w << "," << prob << "]";
    prob_numerator *= prob;
    prob_denominator *= 1-prob;
  }
  if (! is_tested || match_word_count < 2)
    return false;
  res_prob = prob_numerator / (prob_numerator + prob_denominator);
  if (res_prob >= THRESHOLD) {
    Log::Logging(BAYES_DUMP_T, Blog2Str(blog.blog_()) + ">" +  ss.str() + ">" + std::to_string(match_word_count) + ">" +std::to_string(prob_S) + ">" + std::to_string(res_prob));
    return true;
  }
  else return false;
}
#endif

