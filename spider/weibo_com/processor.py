# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import logging
import requests
import re
from collections import namedtuple

from easy_spider import ElementProcessor
from .utils import LoginHandler, PageLoader
from .parser import FriendPageParser
from .element import UrlElement


class UrlProcessor(ElementProcessor):

    # A trick to avoid initialization of _cookies_after_login.
    _cookies_dict = {}
    _cookies_jar = {}

    def _get_login_cookies_dict(self):
        self._cookies_dict = LoginHandler.get_login_cookies_dict()

    def _get_login_cookies_jar(self):
        self._cookies_jar = LoginHandler.get_login_cookies_jar()

    def _check_login_url(self, url):
        return LoginHandler.check_login_url(url)

    def prepare_cookie_and_loader(self):
        self._get_login_cookies_jar()
        self.page_loader = PageLoader(self._cookies_jar)

    def _load_page(self, url):
        return self.page_loader.load_url(url)


class FriendPageProcessor(UrlProcessor):

    def _generate_url_of_fans_page(self, uid):
        URL_TEMPLATE = 'http://weibo.com/{}/follow'
        return URL_TEMPLATE.format(uid)

    def _extract_user_properties(self, response_content):
        # holly shit.
        pattern_template = (
            br"<strong.*?>(\d+)<\\/strong>"
            br"\\r\\n\\t\\t\\t"
            br"<span>{}.*?<"
        )

        def extract_by_key(key):
            pattern = pattern_template.format(key)
            match = re.search(pattern, response_content)
            return match.group(1)

        fans_size = extract_by_key(b"粉丝")
        followers_size = extract_by_key(b"关注")
        messages_size = extract_by_key(b"微博")
        return fans_size, followers_size, messages_size

    def _process_url(self, url):
        # handling return data encapsulation.
        data_interface = namedtuple("_", ['parser', 'extractor'])

        # loading page.
        response_url, response_content = self._load_page(url)
        if self._check_login_url(response_url):
            # refresh cookiesjar and page_loader.
            self.prepare_cookie_and_loader()
            # reload.
            response_url, response_content = self._load_page(url)

        # extract information by parser.
        friend_page_parser = FriendPageParser()
        parser_result = friend_page_parser.parse(url, response_content)

        # get information by extractor.
        extractor_result = self._extract_user_properties(response_content)
        return data_interface(parser=parser_result,
                              extractor=extractor_result)

    def process_element(self, element):
        """
        get url, load page, get fans' uids, build elements.
        """

        result = self._process_url(element.url)
        if result.parser is None:
            return None
        # unpackage.
        uids, next_page, fans_page = result.parser
        fans_size, followers_size, messages_size = result.extractor

        elements = []
        # create element for next_page.
        if next_page:
            elements.append(
                UrlElement(next_page, self),
            )
        # create element for next_page.
        if fans_page:
            elements.append(
                UrlElement(fans_page, self),
            )
        # create new elements.
        for uid in uids:
            url_element = UrlElement(
                self._generate_url_of_fans_page(uid),
                self,
            )
            elements.append(url_element)

        print(uids)
        print(fans_size, followers_size, messages_size)

        return elements


class UserInfoPageProcessor(UrlProcessor):
    pass


class MessagePageProcessor(UrlProcessor):
    pass
