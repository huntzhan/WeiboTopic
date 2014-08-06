
import unittest
import logging
logging.basicConfig(level=20)

from weibo_com.run import run


@unittest.skipIf(True, "It takes times.")
class TestRun(unittest.TestCase):

    def test_run(self):
        run()
