
import unittest
import logging
logging.basicConfig(level=20)
from weibo_cn.run import run


class TestRun(unittest.TestCase):

    def test_run(self):
        run()
