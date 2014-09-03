

class Topic:
    """ @brief one timestamp(one hour) uniquely determines one Topic instance, which usually has many topics
    """
    def __init__(self, _timestamp, _id, **kwargs):
        self.timestamp = _timestamp
        self.id = _id
        self.keywords = kwargs['keywords']
        self.mainidea = kwargs['mainidea']
        self.number_of_blogs = kwargs['weibonumber']
        self.is_politic = kwargs['isPolitic']


class Blog:
    """ @brief Blog instances usually belong to one SubTopic instance
    """
    def __init__(self, _mid, _content, _keywords):
        self.mid = _mid
        self.content = _content
        self.keywords = _keywords


class SubTopic:
    """ @brief timestamp&topic_id may has many SubTopic instances
    """
    def __init__(self, _timestamp, _topic_id, **kwargs):
        self.timestamp = _timestamp
        self.topic_id = _topic_id
        self.number_of_blogs = kwargs['weibonumber']
        self.blogs = kwargs['blogs']


class CachedModel:
    cached_topics = []
    LIMIT = 0
    TOPN = 25
    CACHE_TOPIC_LIMIT = 100

    @classmethod
    def GetOneHourTopic(cls, timestamp):
        """
        @brief construct a 'most-recent-used-first' cached Topic list
        """
        tar = None
        for i in range [0, len(cls.cache_topics)-1]:
            if cls.cache_topics[i].timestamp == timestamp:
                tar = i
        if tar:
            topic = cls.cache_topics[tar]
            del cls.cache_topics[tar]
            cls.cache_topics.insert(0, topic)
        else :
            topic = DBOperator.GetOneHourTopics(timestamp, LIMIT, TOPN)
            cls.cache_topics.insert(0, topic)
            while len(cls.cache_topics) > CACHE_TOPIC_LIMIT:
                cls.cache_topics.pop()


class DBOperator:
    """
    get topics and sub_topics from db throught raw sql queries
    """

    def __init__(self, db_name):
        self.db_name = db_name

    def GetOneHourTopics(self, timestamp, limit, topn):
        """
        @brief read at most 'limit' rows(topics) from db, and
        return the top n based on the number of blogs in each topic
        @input timestamp: format 'xxx'
        @input limit: 0 for no limit
        """
        return []
        pass

    def GetSubTopics(self, timestamp, No_topic, limit):
        """
        @brief read the sub topic tables of one topic at the specific hour determined by timestamp
        @input limit: 0 for no limit
        """
        return [[], []];
        pass
