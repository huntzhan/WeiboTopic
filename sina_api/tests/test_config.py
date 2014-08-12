
from __future__ import (unicode_literals, print_function, absolute_import)


from weibo_crawl.config import read_config
import unittest


class TestConfig(unittest.TestCase):

    def test_config(self):
        l = read_config()
        self.assertTrue(len(l) > 0)
        self.assertTrue('username' in l[0])

if __name__ == '__main__':
    unittest.main()
