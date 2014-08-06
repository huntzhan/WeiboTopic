from __future__ import (unicode_literals, print_function, absolute_import)

import random
from easy_spider import ConcurrentStrategy


class BFS(ConcurrentStrategy):

    def __init__(self):
        self.id_queue_mapping = {}
        self.visited_url = set()

    def _check_and_make_visited(self, url):
        if url in self.visited_url:
            return False
        else:
            self.visited_url.add(url)
            return True

    def valid(self):
        flag = False
        for id, queue in self.id_queue_mapping.items():
            flag = flag or (len(queue) > 0)
        return flag

    def receive_element(self, element, force=False):
        element_id = id(element.processor)
        queue = self.id_queue_mapping.get(element_id, None)
        if queue is None:
            if not force:
                raise Exception("What the fuck.")
            else:
                queue = []
                self.id_queue_mapping[element_id] = queue

        # check visit.
        if self._check_and_make_visited(element.url):
            queue.append(element)

    def next_independent_elements(self, max):
        # select avaliable queue.
        valid_queues = [q for q in
                        self.id_queue_mapping.values() if len(q) > 0]
        random.shuffle(valid_queues)
        # tricks here.
        valid_queues = valid_queues[:max]
        # pop first element of all valid queues.
        result = [q.pop(0) for q in valid_queues]
        return result
