# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import re
import cookielib
import urllib

from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions
from requests.cookies import create_cookie
import mechanize


class _SeleniumOperator(object):

    """
    @brief: Element operations based on selenium.
    """

    @classmethod
    def _find_username_password_elements(cls, driver):
        user_ele = driver.find_element_by_xpath("//input[@id='username']")
        passwd_ele = driver.find_element_by_xpath("//input[@id='password']")
        return user_ele, passwd_ele

    @classmethod
    def _find_verify_element(cls, driver):
        vertify_ele = driver.find_element_by_xpath(
            "//input[@id='door']")
        return vertify_ele

    @classmethod
    def _find_top_level_element(cls, driver):
        return driver.find_element_by_xpath("//html")

    @classmethod
    def _fill_username_password_elements(cls, driver,
                                         user_ele, passwd_ele):
        # USERNAME and PASSWORD is defined as sub-class variables.
        user_ele.clear()
        user_ele.send_keys(cls.USERNAME)
        passwd_ele.clear()
        passwd_ele.send_keys(cls.PASSWORD)

    @classmethod
    def _fill_vertify_element(cls, driver,
                              vertify_ele):
        try:
            vertify_ele.clear()
            keys = raw_input("Enter vertification code> ")
            vertify_ele.send_keys(keys)
        except Exception as e:
            print(e)

    @classmethod
    def _fill_username_and_password(cls, driver):
        user_ele, passwd_ele = cls._find_username_password_elements(driver)
        cls._fill_username_password_elements(driver, user_ele, passwd_ele)
        # remove stupid box.
        passwd_ele.click()

    @classmethod
    def _fill_vertification(cls, driver):
        vertify_ele = cls._find_verify_element(driver)
        cls._fill_vertify_element(driver, vertify_ele)

    @classmethod
    def _submit(cls, driver):
        button = driver.find_element_by_xpath("//input[@type='submit']")
        button.click()

    @classmethod
    def _escape(cls, driver):
        html = cls._find_top_level_element(driver)
        html.send_keys(webdriver.common.keys.Keys.ESCAPE)


class _Adaptor(object):

    """
    @brief: Adaptor functions.
    """

    @classmethod
    def _trans_unicode_element_of_dict(cls, source_dicts):
        trans = lambda x: x.encode("utf-8")
        target_dict = {}
        for cookie_dict in source_dicts:
            key = cookie_dict['name']
            value = cookie_dict['value']
            if isinstance(key, unicode):
                key = trans(key)
            if isinstance(value, unicode):
                value = trans(value)
            target_dict[key] = value
        return target_dict

    @classmethod
    def _trans_cookies_from_dict_to_cookiejar(cls, source_dicts):
        target_cookie_jar = cookielib.CookieJar()

        for cookie_dict in source_dicts:
            # extract name and value.
            name = cookie_dict.pop('name', None)
            value = cookie_dict.pop('value', None)
            # change key name.
            expiry_value = cookie_dict.pop('expiry', None)
            if expiry_value:
                cookie_dict['expires'] = expiry_value

            cookie = create_cookie(name, value, **cookie_dict)
            target_cookie_jar.set_cookie(cookie)

        return target_cookie_jar


class LoginHandler(_SeleniumOperator, _Adaptor):

    """
    @brief: Login to weibo.cn and return cookies.
    """

    LOGIN_URL = "http://login.sina.com.cn/signup/signin.php?entry=sso"
    # Currently set username and password as class variable, should be changed
    # when simulating multiple users.
    USERNAME = "janfancoat2@163.com"
    PASSWORD = "coat123456"

    @classmethod
    def check_login_url(cls, url):
        """
        @return: True if url points to login page.
        """
        LOGIN_URL_PATTERNS = [
            "weibo.com/login",
            "login.sina.com.cn",
            "weibo.com/signup",
        ]
        for pattern in LOGIN_URL_PATTERNS:
            if re.search(pattern, url):
                return True
        return False

    @classmethod
    def _get_login_raw_cookies(cls):
        """
        @brief: Login base on GUI, with consideration of vetification.
        @return: A browser driver was successfully logined.
        """

        driver = webdriver.Firefox()
        wait = WebDriverWait(driver, 10)
        driver.get(cls.LOGIN_URL)
        # try to login without vetification.
        cls._fill_username_and_password(driver)
        cls._submit(driver)

        while True:
            try:
                # wait for page load.
                wait.until(expected_conditions.title_is("新浪通行证"))
                break
            except:
                # still in login page.
                cls._fill_username_and_password(driver)
                cls._fill_vertification(driver)
                cls._submit(driver)
        # After login sina.com, we need to get login session from weibo.com.
        driver.get("http://weibo.com/")
        # wait.until(expected_conditions.title_contains("我的首页"))
        driver.close()

        return driver.get_cookies()

    @classmethod
    def get_login_cookies_dict(cls):
        """
        @return: A dict contains key/value pairs of cookies, which contains
                 bytes instead of unicode.
        """

        raw_cookies = cls._get_login_raw_cookies()
        # extract cookies and return.
        cookies_dict = cls._trans_unicode_element_of_dict(raw_cookies)
        return cookies_dict

    @classmethod
    def get_login_cookies_jar(cls):
        """
        @return: A CookieJar object.
        """

        raw_cookies = cls._get_login_raw_cookies()
        cookiejar = cls._trans_cookies_from_dict_to_cookiejar(raw_cookies)
        return cookiejar


#############
# from cola #
#############
class PageLoader(object):

    def __init__(self, cookie_jar, user_agent=None, timeout=None):

        if user_agent is None:
            user_agent = (
                'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; '
                'rv:31.0) Gecko/20100101 Firefox/31.0'
            )

        self.browser = mechanize.Browser()

        self.cj = cookie_jar
        self.browser.set_cookiejar(self.cj)
        self.browser.set_handle_equiv(True)
        self.browser.set_handle_redirect(True)
        self.browser.set_handle_referer(True)
        self.browser.set_handle_robots(False)
        self.browser.addheaders = [
            ('User-agent', user_agent)]

        if timeout is None:
            self._default_timout =\
                mechanize._sockettimeout._GLOBAL_DEFAULT_TIMEOUT
        else:
            self._default_timout = timeout

    def set_default_timeout(self, timeout):
        self._default_timout = timeout

    def open(self, url, data=None, timeout=None):
        # check if gzip by
        # br.response().info().dict.get('content-encoding') == 'gzip'
        # experimently add `self.br.set_handle_gzip(True)` to handle
        if timeout is None:
            timeout = self._default_timout
        return self.browser.open(url, data=data, timeout=timeout).read()

    def browse_open(self, url, data=None, timeout=None):
        if timeout is None:
            timeout = self._default_timout
        self.browser.open(url, data=data, timeout=timeout)
        return self.browser

    def close(self):
        resp = self.browser.response()
        if resp is not None:
            resp.close()
        self.browser.clear_history()

    def load_url(self, url):
        """
        @brief: adaptor interface.
        @return: current_url, content of response.
        """
        br = self.browse_open(url)
        return br.geturl(), br.response().read()


#############
# from cola #
#############
def beautiful_soup(html, logger=None):
    try:
        from bs4 import BeautifulSoup, FeatureNotFound
    except ImportError:
        raise Exception("BeautifulSoup4")
    try:
        return BeautifulSoup(html, 'lxml')
    except FeatureNotFound:
        return BeautifulSoup(html)


#############
# from cola #
#############
def urldecode(link):
    decodes = {}
    if '?' in link:
        params = link.split('?')[1]
        for param in params.split('&'):
            k, v = tuple(param.split('='))
            decodes[k] = urllib.unquote(v)
    return decodes
