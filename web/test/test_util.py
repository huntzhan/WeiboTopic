
import unittest
from datetime import datetime

from util import ToTimestamp, FromTimestamp


class TestUtil(unittest.TestCase):
    def test_timestamp(self):
        demo_str = '09/02/2014 11:00 PM'
        demo = '1409670000'
        test_str = ToTimestamp(demo_str)
        print test_str
        self.assertTrue(test_str == demo)

    def test_reverse_timestamp(self):
        # for printing only
        # demo = '1410134400'
        demo = '1410159600'
        print FromTimestamp(demo)


if __name__ == '__main__':
    unittest.main()
