# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import logging
import requests
import re
from collections import namedtuple

from easy_spider import ElementProcessor
from .utils import PageLoader
from .cookies_allocator import CookiesAllocator
from .parser import FriendPageParser, MicroBlogParser
from .element import UrlElement
from .persist import WeiboUserHandler


class UrlProcessor(ElementProcessor):

    def __init__(self, *args, **kwargs):
        self.prepare_cookie_and_loader()

    def _check_login_url(self, url):
        return CookiesAllocator.check_login_url(url)

    def prepare_cookie_and_loader(self):
        self._cookies_jar = CookiesAllocator.get_cookies_jar()
        self.page_loader = PageLoader(self._cookies_jar)

    def _load_page(self, url):
        return self.page_loader.load_url(url)

    def _load_page_with_retry(self, url):
        response_url, response_content = self._load_page(url)

        while self._check_login_url(response_url):
            # throw this cookies away.
            CookiesAllocator.set_cookies_jar_invalid(self._cookies_jar)
            # refresh cookiesjar and page_loader.
            self.prepare_cookie_and_loader()
            # reload.
            response_url, response_content = self._load_page(url)
        return response_content


class FriendPageProcessor(UrlProcessor):

    def generate_url_of_fans_page(self, uid):
        URL_TEMPLATE = 'http://weibo.com/{}/follow'
        return URL_TEMPLATE.format(uid)

    def _extract_user_properties(self, response_content):
        # holly shit.
        pattern_template = (
            br"<strong.*?>(\d+)<\\/strong>"
            br"((\\r)|(\\n)|(\\t)|( ))*"
            br"<span>{}.*?<"
        )

        def extract_by_key(key):
            """
            @brief: Let it crash.
            """
            pattern = pattern_template.format(key)
            match = re.search(pattern, response_content)
            number = None if match is None else match.group(1)
            return number

        fans_size = extract_by_key(b"粉丝")
        followees_size = extract_by_key(b"关注")
        messages_size = extract_by_key(b"微博")
        # debug
        if fans_size is None:
            print(response_content)
            raw_input()
        return fans_size, followees_size, messages_size

    def _process_url(self, url):
        # handling return data encapsulation.
        data_interface = namedtuple("_", ['parser', 'extractor'])

        response_content = self._load_page_with_retry(url)

        # extract information by parser.
        friend_page_parser = FriendPageParser()
        parser_result = friend_page_parser.parse(url, response_content)

        # get information by extractor.
        extractor_result = self._extract_user_properties(response_content)

        # package data.
        return data_interface(parser=parser_result,
                              extractor=extractor_result)

    def process_element(self, element):
        """
        get url, load page, get fans' uids, build elements.
        """

        result = self._process_url(element.url)
        if result.parser is None:
            return None
        # unpackage data.
        current_uid, uids, next_page, fans_page = result.parser
        fans_size, followees_size, messages_size = result.extractor

        #######################
        # Database Operations #
        #######################
        handler = WeiboUserHandler()
        if not handler.user_exist(uid=current_uid):
            handler.add_user(
                uid=current_uid,
                followees=followees_size,
                fans=fans_size,
                posts=messages_size,
            )
        elif not handler.user_valid(uid=current_uid):
            handler.update_user(
                uid=current_uid,
                followees=followees_size,
                fans=fans_size,
                posts=messages_size,
            )
        for uid in uids:
            if not handler.user_exist(uid=uid):
                handler.add_user(uid=uid)

        print("#begin#")
        print(current_uid)
        print(fans_size, followees_size, messages_size)
        print(uids)
        print("#end#")

        ########################
        # Processor Operations #
        ########################
        elements = []
        # create element for next_page.
        if next_page:
            new_element = UrlElement(next_page, FriendPageProcessor())
            elements.append(new_element)
        # create element for next_page.
        if fans_page:
            new_element = UrlElement(fans_page, FriendPageProcessor())
            elements.append(new_element)
        # create new elements.
        # for uid in uids:
        #     url_element = UrlElement(
        #         self._generate_url_of_fans_page(uid),
        #         self,
        #     )
        #     elements.append(url_element)
        return elements


class MessagePageProcessor(UrlProcessor):

    def _process_url(self, url):
        response_content = self._load_page_with_retry(url)
        # create parser.
        microblog_parser = MicroBlogParser(url, response_content)
        messages, url_of_next_page = microblog_parser.parse()
        return messages, url_of_next_page

    def process_element(self, element):
        pass
