
import unittest
from collections import defaultdict

from weibo_com.cookies_allocator import CookiesAllocator


SKIP_LOGIN_TEST = True


class TestCookiesSetupMethod(unittest.TestCase):

    username = u"janfancoat2@163.com"
    password = u"coat123456"

    def tearDown(self):
        CookiesAllocator._username_cookies_mapping = defaultdict(list)

    def test_non_gui_base_method(self):
        cookies_jar = CookiesAllocator._non_gui_based_method(
            self.username,
            self.password,
        )
        self.assertGreater(len(cookies_jar), 5)

    @unittest.skipIf(SKIP_LOGIN_TEST, "It takes times.")
    def test_gui_base_method(self):
        cookies_jar = CookiesAllocator._gui_based_method(
            self.username,
            self.password,
        )
        self.assertGreater(len(cookies_jar), 5)

    def test_build_cookeis(self):
        CookiesAllocator.build_cookeis()
        self.assertTrue(CookiesAllocator._check_cookies_availability())
