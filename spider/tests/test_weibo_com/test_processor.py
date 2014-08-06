
import unittest
import cookielib

from weibo_com.processor import UrlProcessor, FriendPageProcessor


SKIP_LOGIN_TEST = True


class TestUrlProcessor(unittest.TestCase):

    def setUp(self):
        self.url_processor = UrlProcessor()

    @unittest.skipIf(SKIP_LOGIN_TEST, "It takes times.")
    def test_cookies_dict(self):
        self.url_processor._get_login_cookies_dict()
        for key, value in\
                self.url_processor._cookies_dict.items():
            self.assertTrue(isinstance(key, str))
            self.assertTrue(isinstance(value, str))

    @unittest.skipIf(SKIP_LOGIN_TEST, "It takes times.")
    def test_cookies_jar(self):
        self.url_processor._get_login_cookies_jar()
        self.assertTrue(
            isinstance(self.url_processor._cookies_jar, cookielib.CookieJar))

    def test_login_url(self):
        not_login_url = "http://weibo.com/u/2243807243"
        self.assertFalse(self.url_processor._check_login_url(not_login_url))

        login_urls = [
            "http://weibo.com/signup/signup.php?inviteCode=3211200050",
            "http://weibo.com/login.php",
        ]
        for url in login_urls:
            self.assertTrue(self.url_processor._check_login_url(url))


class TestFansPage(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.processor = FriendPageProcessor()
        cls.processor.prepare_cookie_and_loader()

    def test_fans_page(self):
        test_url = "http://weibo.com/3211200050/follow?relate=fans"
        result = self.processor._process_url(test_url).parser
        self.assertIsNone(result[-1])

    def test_nonfans_page(self):
        test_url = "http://weibo.com/3211200050/follow"
        result = self.processor._process_url(test_url).parser
        self.assertIsNotNone(result[-1])

    def test_next_page(self):
        test_url = "http://weibo.com/3211200050/follow"
        result = self.processor._process_url(test_url).parser
        self.assertIn('page=2', result[1])

    def test_uids(self):
        test_url = "http://weibo.com/3211200050/follow"
        result = self.processor._process_url(test_url).parser
        self.assertGreater(len(result[0]), 5)

    def test_user_properties(self):
        test_url = "http://weibo.com/3211200050/follow"
        a, b, c = self.processor._process_url(test_url).extractor
        equal_flag = a == b and b == c
        self.assertFalse(equal_flag)
