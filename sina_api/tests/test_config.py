from __future__ import (unicode_literals, print_function, absolute_import)


from weibo_crawl.config import ConfigurationCenter as cc
import unittest


class TestConfig(unittest.TestCase):

    def test_load(self):
        cc.load_configuration()
        self.assertEqual(
            cc._index['2882135661'].get('app_secret'),
            '73418fb9777f309a4fa7b595eb9cf676',
        )


if __name__ == '__main__':
    unittest.main()
