from __future__ import (unicode_literals, print_function, absolute_import)

import logging
import requests
import re

import lxml.html
from easy_spider import ElementProcessor
from .utils import LoginHandler
from .element import UrlElement
from tests.test_weibo_com.opener import MechanizeOpener


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


class FansPageProcessor(UrlProcessor):

    # FANS_URL_TMEPLATE = "http://weibo.cn/{}/fans?vt=4"

    # def _get_page_content(self, url):
    #     response = requests.get(
    #         url,
    #         cookies=self._cookies_after_login,
    #     )
    #     return response.url, response.text

    # def _extract_uids(self, text):
    #     # pattern = re.compile(r".*?weibo.cn/u/(\d+).*?", re.UNICODE)
    #     # for match in pattern.finditer(text):
    #     #     uid = match.group(1)
    #     #     yield uid
    #     tree = lxml.html.fromstring(text.encode("utf-8"))
    #     target_input_element = tree.xpath("//input[@name='uidList']")[0]

    #     uids = target_input_element.attrib['value']
    #     logging.info("Current uids: %s", uids)
    #     for uid in uids.split(','):
    #         yield uid

    # def _extract_number_of_fans(self, text):
    #     tree = lxml.html.fromstring(text.encode("utf-8"))
    #     target_element = tree.xpath("//span[@class='tc']")[0]

    #     match = re.search(r".*?(\d+).*?", target_element.text, re.UNICODE)
    #     if match:
    #         size = match.group(1)
    #         logging.info("Size of fans: %s", size)
    #         return int(size)
    #     else:
    #         logging.error("Fans size error: %s", target_element.text)
    #         return None

    # def process_element(self, element):
    #     url, content = self._get_page_content(element.url)
    #     if self._check_login_url(url):
    #         self._get_login_cookies()
    #         # reload page.
    #         url, content = self._get_page_content(element.url)

    #     if self._check_login_url(url):
    #         # still error.
    #         logging.error("Can't login.")
    #         return None

    #     fans_size = self._extract_number_of_fans(content)
    #     if not fans_size:
    #         # cases that fans_size is None or fans_size is 0.
    #         return None

    #     new_elements = []
    #     for uid in self._extract_uids(content):
    #         new_url = self.FANS_URL_TMEPLATE.format(uid)
    #         url_element = UrlElement(new_url)
    #         url_element.set_processor(self)
    #         new_elements.append(url_element)
    #     return new_elements
    pass


class UserInfoPageProcessor(UrlProcessor):
    pass


class MessagePageProcessor(UrlProcessor):
    pass
