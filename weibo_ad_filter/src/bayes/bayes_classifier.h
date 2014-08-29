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
                   const std::list<shared_ptr<ParsedBlog>> &all_blogs,
                   shared_ptr<std::map<string, double>> dict,
                   const double threshold) : THRESHOLD(threshold) { 
      word_dict = dict;
      cata_size = cata_blogs.size();
      prob_S = cata_size*1.0 / all_blogs.size();
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
      unsigned count_lower_bound = static_cast<unsigned>(cata_size * SPAM_WORD_LOWER_BOUND_RATIO);
      for (auto &kv : counter) {
        if (kv.second < count_lower_bound)
          continue;
        prob_w_given_S[kv.first] = kv.second*1.0 / cata_size;
      }
    }
    ~Bayes() {}

    bool IsSpam(ParsedBlog &blog, double &res_prob) {
      double prob_numerator = 1,
             prob_denominator = 1;
      bool is_tested = false;
      for (auto &w : blog.Towords()) {
        double prob_w = (*word_dict)[w];
        if (prob_w_given_S.find(w) == prob_w_given_S.end())
          continue;
        is_tested = true;
        double prob = prob_w_given_S[w]*prob_S / prob_w;
        prob_numerator *= prob;
        prob_denominator *= 1-prob;
      }
      if (! is_tested)
        return false;
      res_prob = prob_numerator / prob_denominator;
      Log::Logging(BAYES_T, Blog2Str(blog.blog_()) + ">" +  GetSpamWords() + ">" + std::to_string(prob_S) + ">" + std::to_string(res_prob));
      if (res_prob >= THRESHOLD)
        return true;
      else return false;
    }

    string GetSpamWords() {
      std::ostringstream ss;
      for (auto &kv : prob_w_given_S) {
        ss<<"[" << kv.first << "," << kv.second << "]";
      }
      return ss.str();
    }

  private:
    const double THRESHOLD;
    const double SPAM_WORD_LOWER_BOUND_RATIO = 0.5;
    unsigned cata_size;
    std::shared_ptr<std::map<string, double>> word_dict;
    double prob_S;
    std::map<string, double> prob_w_given_S;

    // DISALLOW_COPY_AND_ASSIGN
    Bayes(const Bayes&);
    void operator=(const Bayes&);
};
#endif

