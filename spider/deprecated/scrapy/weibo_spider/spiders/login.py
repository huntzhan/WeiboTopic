# -*- coding: utf-8 -*-

import scrapy
from scrapy.contrib.spiders import Rule, CrawlSpider
from scrapy.contrib.linkextractors import LinkExtractor
from scrapy.http import Request

from cola_opener import MechanizeOpener
from cola_login import WeiboLogin


class LoginSpider(CrawlSpider):
    name = "login"
    # allowed_domains = ["www.weibo.com"]
    start_urls = (
        "http://weibo.com/guide/welcome",
    )

    rules = (
        Rule(LinkExtractor(allow='login'),
             callback='login_prepare'),
        Rule(LinkExtractor(allow='follow'),
             callback='parse_follower'),
    )

    def login_prepare(self, response):
        user = "13570322748"
        passwd = "huntzhan"
        cola_opener = MechanizeOpener()
        cola_login = WeiboLogin(cola_opener, user, passwd)
        flag = cola_login.login()
        # get cookies.
        print "###############"
        print flag
        print cola_opener.cj
        login_cookies = {item.name: item.value for item in cola_opener.cj}
        print login_cookies
        print "###############"
        result = [
            Request('http://weibo.com/3211200050/follow', cookies=login_cookies)
            for _ in range(1000)
        ]
        return result

    def parse_follower(self, response):
        return Request('http://weibo.com/3211200050/follow')
