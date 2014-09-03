
import unittest
from datetime import datetime

from util import ToTimestamp


class TestUtil(unittest.TestCase):
    def test_timestamp(self):
        demo_str = '09/02/2014 11:00 PM'
        demo = '1409670000'
        test_str = ToTimestamp(demo_str)
        print test_str
        self.assertTrue(test_str == demo)

        demo_str = "09/02/2014 11:14 PM"
        demo = '1409670000'
        test_str = ToTimestamp(demo_str)
        print test_str
        self.assertTrue(test_str == demo)
        # secs = float(demo)
        # dt = datetime.fromtimestamp(secs)
        # date_str = dt.strftime('%d/%m/%Y %H:%M')
        # self.assertTrue(date_str == demo_str)


if __name__ == '__main__':
    unittest.main()
