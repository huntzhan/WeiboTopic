
import unittest
from weibo_com.cookies_allocator import CookiesAllocator


class TestCookiesSetupMethod(unittest.TestCase):

    username = u"janfancoat2@163.com"
    password = u"coat123456"

    def test_non_gui_base_method(self):
        cookies_jar = CookiesAllocator._non_gui_based_method(
            self.username,
            self.password,
        )
        self.assertGreater(len(cookies_jar), 5)

    def test_gui_base_method(self):
        cookies_jar = CookiesAllocator._gui_based_method(
            self.username,
            self.password,
        )
        self.assertGreater(len(cookies_jar), 5)
