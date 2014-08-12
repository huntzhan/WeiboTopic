from __future__ import (unicode_literals, print_function, absolute_import)


from weibo_crawl.bussiness import Schedule
from pprint import pprint
import unittest


SKIP_LONG_TEST = True


class TestSchedule(unittest.TestCase):

    @unittest.skipIf(SKIP_LONG_TEST, "It takes times.")
    def test_run(self):
        schedule = Schedule(5)
        for index in range(20):
            schedule.add_callback(pprint, index)
        schedule.run()
