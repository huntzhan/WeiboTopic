# -*- coding: utf-8 -*-

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
             self.mainidea.encode('utf8'), self.keywords.encode('utf8'))


class Blog:
    """ @brief Blog instances usually belong to one SubTopic instance
    """
    def __init__(self, _content, _keywords):
        self.content = _content
        self.keywords = _keywords

    def jsonify(self):
        return {
            'content': self.content,
            'keywords': self.keywords
        }

    def __repr__(self):
        return "content: %s\nkeywords: %s\n" %\
            (self.content.encode('utf8'),
             self.keywords.encode('utf8'))


class SubTopic:
    """
    @brief timestamp&topic_id may has many SubTopic instances
    """
    def __init__(self, _timestamp, _topic_id, **kwargs):
        self.timestamp = _timestamp
        self.topic_id = _topic_id
        self.number_of_blogs = kwargs['weibonumber']
        self.blogs = kwargs['blogs']

    def jsonify(self):
        blogs = []
        blogs_count = 0
        for blog in self.blogs:
            blogs.append(blog.jsonify())
        return {
            'timestamp': self.timestamp,
            'topic_id': self.topic_id,
            'number_of_blogs': self.number_of_blogs,
            'blogs': blogs
        }

    def __repr__(self):
        s = "%s_%d, number: %d\nblogs:\n" %\
            (self.timestamp.encode('utf8'),
             self.topic_id,
             self.number_of_blogs)
        for blog in self.blogs:
            s += str(blog)
        return s


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
        for i in range(0, len(cls.cached_topics)):
            if (cls.cached_topics[i][0]).timestamp == timestamp:
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
        # keywords = row[1].encode('utf-8')
        # mainidea = row[2].encode('utf-8')
        keywords = row[1]
        mainidea = row[2]
        # keywords = unicode(row[1], 'utf8')
        # mainidea = unicode(row[2], 'utf8')
        number = row[3]
        is_politic = row[4]
        return Topic(timestamp,
                     topic_id,
                     **{'keywords':keywords,
                        'mainidea':mainidea,
                        'weibonumber':number,
                        'isPolitic':is_politic})

    @classmethod
    def GetSubTopics(cls, timestamp, No_topic):
        """
        @brief return None if no sub topics
        """
        sub_topic_tables = cls.db_handler.GetSubTopics(timestamp, No_topic)
        if len(sub_topic_tables) == 0:
            return None
        sub_topics = []
        for sub_topic in sub_topic_tables:
            sub_topics.append(cls._MakeSubTopicFromRows(sub_topic, timestamp, No_topic))
        return sub_topics

    @classmethod
    def _MakeSubTopicFromRows(cls, rows, timestamp, No_topic):
        blogs = []
        for row in rows:
            blogs.append(cls._MakeBlogFromRow(row));
            subtopic = SubTopic(timestamp, No_topic, **{'weibonumber': len(blogs), 'blogs': blogs})
        return subtopic

    @classmethod
    def _MakeBlogFromRow(cls, row):
        blog = Blog(row[1], row[2])
        return blog
