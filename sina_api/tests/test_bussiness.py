from __future__ import (unicode_literals, print_function, absolute_import)


from weibo_crawl.bussiness import (Schedule, WeiboAPIHandler,
                                   PublicTimelineQuery)
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


class TestWeiboAPIHandler(unittest.TestCase):

    def setUp(self):
        self.handler = WeiboAPIHandler(
            'vproject_janfan2@163.com',
            'tencent',
            '18331851',
            'db5b280e0620978bbd278c0bc630945d',
            '3043df03489bc77c706c2c2bbad71e52',
            '2.00mXJLjF0BxuOB86e1b004c7ZE16KC',
            'http://haoxun.org',
        )

    @unittest.skipIf(SKIP_LONG_TEST, "It takes times.")
    def test_apply(self):
        handler = self.handler
        handler._apply_for_access_token()
        self.assertEqual(len(handler.access_token), 32)

    @unittest.skipIf(SKIP_LONG_TEST, "It takes times.")
    def test_get_code(self):
        handler = self.handler
        code = handler._get_code()
        self.assertEqual(len(code), 32)

    @unittest.skipIf(SKIP_LONG_TEST, "It takes times.")
    def test_apply(self):
        handler = self.handler
        data = handler.apply(
            "https://api.weibo.com/2/statuses/public_timeline.json",
            count=200,
        )
        self.assertEqual(data['total_number'], 200)

    @unittest.skipIf(SKIP_LONG_TEST, "It takes times.")
    def test_multiple_apply(self):
        h1 = self.handler
        h2 = WeiboAPIHandler(
            'vproject_janfan2@163.com',
            'tencent',
            '18331851',
            'db5b280e0620978bbd278c0bc630945d',
            '',
            '',
            'http://haoxun.org',
        )
        handlers = [h1, h2]

        for handler in handlers:
            data = handler.apply(
                "https://api.weibo.com/2/statuses/public_timeline.json",
                count=200,
            )
            num = data.get('total_number', None)
            self.assertEqual(num, 200)

    def test_retewwt(self):
        h1 = self.handler
        query = PublicTimelineQuery(h1)
        query.query()
