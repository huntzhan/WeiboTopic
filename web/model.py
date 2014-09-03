
from db import DBOperator

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

    def __repr__(self):
        return "%s-%d, number: %d, is political: %d\n%s\n%s\n" %\
            (self.timestamp, self.id,
             self.number_of_blogs, self.is_politic,
             self.mainidea, self.keywords)


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
    db_handler = DBOperator()
    # TOPN = 25
    CACHE_TOPIC_LIMIT = 100

    @classmethod
    def GetOneHourTopic(cls, timestamp):
        """
        @brief construct a 'most-recent-used-first' cached Topic list
        @return [] if table doesn't exist
        """
        tar = None
        for i in range(0, len(cls.cached_topics)-1):
            if cls.cached_topics[i].timestamp == timestamp:
                tar = i
        topics = []
        if tar:
            topics = cls.cached_topics[tar]
            del cls.cached_topics[tar]
            cls.cached_topics.insert(0, topics)
        else :
            rows = cls.db_handler.GetOneHourTopics(timestamp)
            if rows:
                for row in rows:
                    topics.append(cls._MakeTopicFromRow(row, timestamp))
                cls.cached_topics.insert(0, topics)
                while len(cls.cached_topics) > cls.CACHE_TOPIC_LIMIT:
                    cls.cached_topics.pop()
        return topics

    @classmethod
    def _MakeTopicFromRow(cls, row, timestamp):
        topic_id = row[0]
        keywords = row[1].encode('utf-8')
        mainidea = row[2].encode('utf-8')
        number = row[3]
        is_politic = row[4]
        return Topic(timestamp,
                     topic_id,
                     **{'keywords':keywords,
                        'mainidea':mainidea,
                        'weibonumber':number,
                        'isPolitic':is_politic})
