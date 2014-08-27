/**
 * @file    tactic.h
 * @author  Guangyi Zhang <guangyizhang.jan@gmail.com>
 * @date    08/19/2014 06:48:08 PM
 * @version 1.0
 *
 * @section LICENSE
 * 
 */

#ifndef tactic_INC
#define tactic_INC
#include <set>
#include "split/parser.h"
#include "db/model.h"
#include "simhash/simhash.h"
#include "logger/log.h"
using std::set;
/**
 *  @brief Tactic Base class for different tactic performed on every single message
 */
class Tactic {
public:
  unsigned int tactic_count;
  // ====================  LIFECYCLE     ==================================
  Tactic() {
    tactic_count = 0;
  }
  ~Tactic() {}
  virtual bool IsSpam(const Blog &b) = 0;
  unsigned int count() {
    return tactic_count;
  }

private:

  // DISALLOW_COPY_AND_ASSIGN
  Tactic(const Tactic&);
  void operator=(const Tactic&);
};


/**
 *  @brief ZombieTactic 
 */
class ZombieTactic : public Tactic {
  public:
    // ====================  LIFECYCLE     ==================================
    ZombieTactic() {}
    ~ZombieTactic() {}
    virtual bool IsSpam(const Blog &b);

  private:

    // DISALLOW_COPY_AND_ASSIGN
    ZombieTactic(const ZombieTactic&);
    void operator=(const ZombieTactic&);
};


/**
 *  @brief SourceTactic 
 */
class SourceTactic : public Tactic {
  public:
    // ====================  LIFECYCLE     ==================================
    SourceTactic() {}
    ~SourceTactic() {
      for (auto i : counter) {
        Log::Logging(TACTIC_SOURCE_T, i.first + ">" + std::to_string(i.second));
      }
    }
	virtual bool IsSpam(const Blog &b) {
      if (counter.find(b.m_source) == counter.end()) {
        counter[b.m_source] = 1;
      }
      else {
        counter[b.m_source] += 1;
      }
      if (b.m_source == "微问") {
        tactic_count++;
        return true;
      }
      return false;
    }

  private:
    std::map<string, unsigned int> counter;
    // DISALLOW_COPY_AND_ASSIGN
    SourceTactic(const SourceTactic&);
    void operator=(const SourceTactic&);
};

class TopicTcatic : public Tactic{
  public:
	 // ====================  LIFECYCLE     ==================================
	static std::vector<std::set<std::string> > badTopic;

	void GetBadTopic(std::vector<std::set<std::string> > &input);
	TopicTcatic();
	~TopicTcatic(){}
	virtual bool IsSpam(const Blog &b);
  private:
    int match;
	TopicTcatic(const TopicTcatic &);
	void operator=(const TopicTcatic &);
};

class UserTactic : public Tactic{
  public:
	static std::set<std::string> baduser;
	void GetBadUser(std::set<std::string> &input);
	UserTactic();
	~UserTactic(){}
	virtual bool IsSpam(const Blog &b);
  private:
	UserTactic(const UserTactic &);
	void operator=(const UserTactic &);
};

#endif
