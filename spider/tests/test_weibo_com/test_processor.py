
import unittest
from weibo_com.processor import UrlProcessor


class TestUrlProcessor(unittest.TestCase):

    def setUp(self):
        self.url_processor = UrlProcessor()

    def test_cookies_dict(self):
        self.url_processor._get_login_cookies_dict()
        for key, value in\
                self.url_processor._cookies_dict.items():
            self.assertTrue(isinstance(key, str))
            self.assertTrue(isinstance(value, str))

    def test_cookies_jar(self):
        self.url_processor._get_login_cookies_jar()

    def test_login_url(self):
        is_login_url = "http://weibo.com/login.php"
        not_login_url = "http://weibo.com/u/2243807243"
        self.assertTrue(self.url_processor._check_login_url(is_login_url))
        self.assertFalse(self.url_processor._check_login_url(not_login_url))
