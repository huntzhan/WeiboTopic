from __future__ import (unicode_literals, print_function, absolute_import)

from threading import Timer
from pprint import pprint
import time


class Schedule(object):

    def __init__(self, duration):
        self.callback_argument_queue = []
        self.duration = duration

    def add_callback(self, callback, *args, **kwargs):
        self.callback_argument_queue.append(
            (callback, args, kwargs),
        )

    def run(self):
        wait_time_per_callback =\
            float(self.duration) / len(self.callback_argument_queue)
        current_wait_time = wait_time_per_callback

        tasks = []
        for callback, args, kwargs in self.callback_argument_queue:
            timer = Timer(
                current_wait_time,
                callback,
                args=args,
                kwargs=kwargs,
            )
            tasks.append(timer)
            # update current_wait_time.
            current_wait_time += wait_time_per_callback

        # start timer.
        for timer in tasks:
            timer.start()
        # just wait.
        time.sleep(self.duration)
