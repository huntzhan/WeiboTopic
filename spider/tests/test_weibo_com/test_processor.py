
import unittest
import cookielib

from weibo_com.processor import UrlProcessor
from tests.test_weibo_com.opener import MechanizeOpener


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


# @unittest.skipIf(SKIP_LOGIN_TEST, "It takes times.")
class TestLoignedJar(unittest.TestCase):

    def setUp(self):
        url_processor = UrlProcessor()
        url_processor._get_login_cookies_jar()

        self.cookiejar = url_processor._cookies_jar
        self.check_login = url_processor._check_login_url

    def test_cookie_jar(self):
        opener = MechanizeOpener(self.cookiejar)
        test_url = "http://weibo.com/3211200050/follow?relate=fans"
        br = opener.browse_open(test_url)
        content = br.response().read()

        # print content
        self.assertIn("FM.view", content)
        self.assertFalse(
            self.check_login(br.geturl()),
        )
