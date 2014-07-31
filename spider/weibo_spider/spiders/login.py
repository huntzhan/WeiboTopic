# -*- coding: utf-8 -*-

import scrapy
from scrapy.contrib.spiders import Rule, CrawlSpider
from scrapy.contrib.linkextractors import LinkExtractor

from cola_opener import MechanizeOpener
from cola_login import WeiboLogin


class LoginSpider(CrawlSpider):
    name = "login"
    # allowed_domains = ["www.weibo.com"]
    start_urls = (
        'http://www.qq.com',
    )

    rules = (
        Rule(LinkExtractor(allow='qq.com'),
             callback='login_prepare'),
    )

    def login_prepare(self, response):
        user = "wtf9279@163.com"
        passwd = "huntzhan"
        cola_opener = MechanizeOpener()
        cola_login = WeiboLogin(cola_opener, user, passwd)
        flag = cola_login.login()
        print "#########################"
        if flag:
            print "SUCCESS"
        else:
            print "FAIL"
        print "#########################"
