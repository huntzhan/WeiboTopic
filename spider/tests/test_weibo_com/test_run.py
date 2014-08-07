
import unittest
import logging
logging.basicConfig(level=20)

from weibo_com.run import run
from weibo_com.model import DB


class TestRun(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        DB.open()

    @unittest.skipIf(True, "It takes times.")
    def test_run(self):
        run()

    @classmethod
    def tearDownClass(cls):
        DB.close()
