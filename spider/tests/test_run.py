
import unittest
import logging
logging.basicConfig(level=20)

from weibo_com.run import run


class TestRun(unittest.TestCase):

    # @unittest.skipIf(True, "It takes times.")
    def test_run(self):
        run()

if __name__ == '__main__':
    unittest.main()
