from __future__ import (unicode_literals, print_function, absolute_import)


class WeiboUser():

    EMPTY = -1

    def __init__(self, uid, followees=EMPTY, fans=EMPTY, posts=EMPTY):
        self.uid = uid
        self.followees = followees
        self.fans = fans
        self.posts = posts


class Microblog():

    EMPTY = -1

    def __init__(self,
                 mid,
                 created_time,
                 content,
                 favorites,
                 comments,
                 forwards,
                 forwarded_content=None):
        self.mid = mid
        self.created_time = created_time
        self.content = content
        self.favorites = favorites
        self.comments = comments
        self.forwards = forwards
        self.forwarded_content = forwarded_content
