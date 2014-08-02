
import unittest
from weibo_cn.processor import UrlProcessor


class TestUrlProcessor(unittest.TestCase):

    def setUp(self):
        self.url_processor = UrlProcessor()

    def test_cookies(self):
        self.url_processor._get_login_cookies()
        for key, value in\
                self.url_processor._cookies_after_login.items():
            self.assertTrue(isinstance(key, str))
            self.assertTrue(isinstance(value, str))

    def test_login_url(self):
        is_login_url = "http://login.weibo.cn/login/?a=1&b=2"
        not_login_url = "http://weibo.cn/1191258123/fans"
        self.assertTrue(self.url_processor._check_login_url(is_login_url))
        self.assertFalse(self.url_processor._check_login_url(not_login_url))
