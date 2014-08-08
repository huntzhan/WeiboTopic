from __future__ import (unicode_literals, print_function, absolute_import)


class WeiboUser():

    EMPTY = -1

    def __init__(self, uid, followees=EMPTY, fans=EMPTY, posts=EMPTY):
        self.uid = uid
        self.followees = followees
        self.fans = fans
        self.posts = posts
