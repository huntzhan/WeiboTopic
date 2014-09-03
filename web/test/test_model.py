
import unittest
from model import CachedModel


class TestModel(unittest.TestCase):
    def test_cachemodel(self):
        demo = '1409670000'
        topics = CachedModel.GetOneHourTopic(demo)
        for t in topics:
            print t
            print ''


if __name__ == '__main__':
    unittest.main()
