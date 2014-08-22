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
using std::set;
/**
 *  @brief Tactic Base class for different tactic performed on every single message
 */
class Tactic {
public:
  // ====================  LIFECYCLE     ==================================
  Tactic() {}
  ~Tactic() {}
  virtual bool IsSpam(const Blog &b) = 0;

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
    bool IsBlogInFingerprints(const Blog &b, int dist);
    bool IsSimhashValuesInDB(vector<unsigned int> v){return false;}
    void FlushCachedFingerprint(int dist);
    void AddFingerPrint(const Blog &b, int dist);
    void Flush(vector<unsigned int> &v){}  /*  Flush the simhash value in set into db */
    set<unsigned int> fingerprint;
    SimHash sim;
    const size_t FLUSH_DB_THRED = 10000;

    // DISALLOW_COPY_AND_ASSIGN
    ZombieTactic(const ZombieTactic&);
    void operator=(const ZombieTactic&);
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
