from __future__ import (unicode_literals, print_function, absolute_import)

import time
from easy_spider import Strategy


class BFS(Strategy):

    def __init__(self):
        self.queue = []

    def valid(self):
        if len(self.queue) > 0:
            return True
        return False

    def receive_element(self, element):
        self.queue.append(element)

    def next_element(self):
        # time.sleep(0.1)
        return self.queue.pop(0)
