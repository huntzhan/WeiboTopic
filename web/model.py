

class Topic:
    def __init__(self, _summary, _keywords, number_of_blogs, sub_topics):
        self.summary = _summary
        self.keywords = _keywords
        self.number_of_blogs = number_of_blogs
        self.sub_topics = sub_topics


class SubTopic:
    def __init__(self, _keywords, _number_of_blogs, blogs):
        self.keywords = _keywords
        self.number_of_blogs = _number_of_blogs
        self.blogs = blogs
