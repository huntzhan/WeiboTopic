
import unittest
from datetime import datetime

from util import ToTimestamp


class TestUtil(unittest.TestCase):
    def test_timestamp(self):
        demo_str = '02/09/2014 23:00'
        demo = '1409670000'
        secs = float(demo)
        dt = datetime.fromtimestamp(secs)
        # format '09/10/2014 02:52'
        date_str = dt.strftime('%d/%m/%Y %H:%M')
        self.assertTrue(date_str == demo_str)
        self.assertTrue(ToTimestamp(demo_str) == demo)


if __name__ == '__main__':
    unittest.main()
