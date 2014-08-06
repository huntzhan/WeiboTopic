
import unittest

from weibo_com.config import Config


class TestConfig(unittest.TestCase):

    def setUp(self):
        pass

    def test_config(self):
        dict_db = Config.values
        self.assertTrue(dict_db['db_name'] == 'sina')
