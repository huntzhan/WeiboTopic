from __future__ import (unicode_literals, print_function, absolute_import)

import random

from easy_spider import ConcurrentStrategy
from .persist import WeiboUserHandler
from .processor import FriendPageProcessor, MessagePageProcessor
from .element import UrlElement


class RandomInvalidUserAccessor(ConcurrentStrategy):
    """
    @brief: a specific strategy for crawling user information. The processor
            bounded to this strategy should not generate "new" elements. "New"
            elements would be generated though database query.
    """

    def __init__(self):
        self.queue = []
        self.visited_url = set()

    def valid(self):
        actual_size, uids = WeiboUserHandler.get_invalid_user(10)
        return actual_size > 0 or len(self.queue) > 0

    def receive_element(self, element):
        if element.url not in self.visited_url:
            self.queue.append(element)
            self.visited_url.add(element.url)
        else:
            print("Already visited: ", element.url)

    def next_independent_elements(self, max):
        # select avaliable queue.
        if len(self.queue) >= max:
            result = self.queue[:max]
            self.queue = self.queue[max:]
            return result
        else:
            diff = max - len(self.queue)
            # generate elements.
            elements = []
            elements.extend(self.queue)
            self.queue = []
            # query for invalid uid to db.
            actual_size, uids = WeiboUserHandler.get_invalid_user(diff)
            uids = uids[:diff]

            for uid in uids:
                url = FriendPageProcessor.generate_url_of_fans_page(uid)
                element = UrlElement(url, FriendPageProcessor())
                elements.append(element)
            return elements


class RandomMicroBlogAccessor(ConcurrentStrategy):

    def __init__(self):
        self.queue = []

    def valid(self):
        pass

    def receive_element(self, element):
        self.queue.append(element)

    def next_independent_elements(self, max):
        pass
