# -*- coding: utf-8 -*-

import unittest

from weibo_com.processor import UrlProcessor
from weibo_com.opener import MechanizeOpener
from weibo_com.parser import UserFriendParser
from weibo_com.weibo_user import WeiboUserBundle


class TestFans(unittest.TestCase):

    def setUp(self):
        url_processor = UrlProcessor()
        url_processor._get_login_cookies_jar()

        self.cookiejar = url_processor._cookies_jar

    def test_fans(self):
        opener = MechanizeOpener(self.cookiejar)
        bundle = WeiboUserBundle('3211200050')
        urls = bundle.urls()
        page_limit = 10
        while len(urls) > 0 and page_limit > 0:
            print '--------------------- Page ----------------------'
            url = urls.pop(0)
            parser = UserFriendParser(opener, url, bundle=bundle)
            next_urls, bundles = parser.parse()
            urls.extend(next_urls)
            page_limit -= 1
            for b in bundles:
                print b.uid


if __name__ == "__main__":
    unittest.main()
