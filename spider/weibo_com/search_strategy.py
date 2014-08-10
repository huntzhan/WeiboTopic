from __future__ import (unicode_literals, print_function, absolute_import)

import random
from easy_spider import ConcurrentStrategy


class RandomInvalidUserAccessor(ConcurrentStrategy):
    """
    @brief: a specific strategy for crawling user information. The processor
            bounded to this strategy should not generate "new" elements. "New"
            elements would be generated though database query.
    """

    def __init__(self):
        self.queue = []

    def valid(self):
        return len(self.queue) > 0

    def receive_element(self, element):
        self.append(element)

    def next_independent_elements(self, max):
        # select avaliable queue.
        if len(self.queue) >= max:
            result = self.queue[:max]
            self.queue = self.queue[max:]
            return result
        else:
            diff = max - len(self.queue)
            # query for invalid uid to db.
            pass
