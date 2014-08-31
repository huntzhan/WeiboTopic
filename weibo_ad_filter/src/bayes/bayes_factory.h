/**
 * @file    bayes_factory.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/31/2014 03:36:26 PM
 * @version 1.0
 *
 */

#ifndef bayes_factory_INC
#define bayes_factory_INC

#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <utility>
#include "logger/log.h"
#include "bayes/bayes_classifier.h"
using std::string;
using std::cout;
using std::endl;
using std::vector;

/**
 *  @brief BayesFactory 
 */
class BayesFactory {
  public:
    std::list<shared_ptr<Bayes>> MakeBayesClassifiers(std::list<shared_ptr<ParsedBlog>> &blogs) {
      /// set up catalogs
      std::map<unsigned, std::list<shared_ptr<ParsedBlog>>> catalogs;
      for (auto &blog : blogs) {
        string content= (blog->blog_()).m_content;
        if (content.find("#") != string::npos) /// dont take official topics as bayes classifier
          continue;
        catalogs[blog->match_fingerprint_()].push_back(blog);
      }
      /// set up words dict
      auto dict = _MakeWordDict(blogs);
      /// set up bayes
      std::list<shared_ptr<Bayes>> classifiers;
      for (auto &kv : catalogs) {
        auto &catalog = kv.second;
        if (catalog.size() < BAYES_REF_COUNT_LOWER_BOUND)
          continue;
        std::shared_ptr<Bayes> bayes(new Bayes(catalog, parsed_blogs.size(), word_dict, BAYES_SPAM_THRESHOLD));
        classifiers.push_back(bayes);
      }
      Log::Logging(RUN_T, "###Bayes has setup: " + std::to_string(classifiers.size()));
      Log::Logging(BAYES_T, "###Bayes has setup: " + std::to_string(classifiers.size()));
      return classifiers;
    }

    // ====================  LIFECYCLE     ==================================
    BayesFactory() {}
    ~BayesFactory() {}

  private:
    std::shared_ptr<std::map<string, double>> _MakeWordDict(std::list<shared_ptr<ParsedBlog>> &parsed_blogs) {
      shared_ptr<std::map<string, double>> dict(new std::map<string, double>);
      /// calculate words_dict, i.e. P(w)
      for (auto &blog : parsed_blogs) {
        for (auto &w : blog->Towords()) {
          if (dict->find(w) == dict->end())
            dict->insert(std::pair<string, unsigned>(w, 1));
          else dict->at(w) += 1;
        }
      }
      unsigned all_blogs = parsed_blogs.size();
      for (auto &kv : *dict) {
        kv.second = kv.second / all_blogs;
      }
      return dict;
    }

    const unsigned BAYES_REF_COUNT_LOWER_BOUND = 5;
    const double BAYES_SPAM_THRESHOLD = 0.8;

    // DISALLOW_COPY_AND_ASSIGN
    BayesFactory(const BayesFactory&);
    void operator=(const BayesFactory&);
};
#endif
