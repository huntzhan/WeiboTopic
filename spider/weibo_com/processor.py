from __future__ import (unicode_literals, print_function, absolute_import)

import logging
import requests
import re

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


class FriendPageProcessor(UrlProcessor):

    def prepare_cookie_and_loader(self):
        self._get_login_cookies_jar()
        self.page_loader = PageLoader(self._cookies_jar)

    def _load_page(self, url):
        return self.page_loader.load_url(url)

    def _process_url(self, url):
        # loading page.
        response_url, response_content = self._load_page(url)
        if self._check_login_url(response_url):
            self._get_login_cookies_jar()
            response_url, response_content = self._load_page(url)

        # extract information.
        friend_page_parser = FriendPageParser()
        result = friend_page_parser.parse(url, response_content)
        return result

    def process_element(self, element):
        """
        get url, load page, get fans' uids, build elements.
        """
        return None


class UserInfoPageProcessor(UrlProcessor):
    pass


class MessagePageProcessor(UrlProcessor):
    pass
