# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import base64
import binascii
import cookielib
import json
import re
import urllib

import mechanize
from requests.cookies import create_cookie
import rsa
from selenium import webdriver
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions


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
                                         user_ele, passwd_ele,
                                         username, password):
        # USERNAME and PASSWORD is defined as sub-class variables.
        user_ele.clear()
        user_ele.send_keys(username)
        passwd_ele.clear()
        passwd_ele.send_keys(password)

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
    def _fill_username_and_password(cls, driver,
                                    username, password):
        user_ele, passwd_ele = cls._find_username_password_elements(driver)
        cls._fill_username_password_elements(
            driver, user_ele, passwd_ele,
            username, password,
        )
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


class GUIBasedLogin(_SeleniumOperator, _Adaptor):

    """
    @brief: Login to weibo.cn and return cookies.
    """

    LOGIN_URL = "http://login.sina.com.cn/signup/signin.php?entry=sso"

    @classmethod
    def _get_login_raw_cookies(cls, username, password):
        """
        @brief: Login base on GUI, with consideration of vetification.
        @return: A browser driver was successfully logined.
        """

        driver = webdriver.Firefox()
        wait = WebDriverWait(driver, 10)
        driver.get(cls.LOGIN_URL)
        # try to login without vetification.
        cls._fill_username_and_password(driver, username, password)
        cls._submit(driver)

        while True:
            try:
                # wait for page load.
                wait.until(expected_conditions.title_is("新浪通行证"))
                break
            except:
                # still in login page.
                cls._fill_username_and_password(driver, username, password)
                cls._fill_vertification(driver)
                cls._submit(driver)
        # After login sina.com, we need to get login session from weibo.com.
        driver.get("http://weibo.com/")
        # wait.until(expected_conditions.title_contains("我的首页"))
        driver.close()

        return driver.get_cookies()

    @classmethod
    def get_login_cookies_dict(cls, username, password):
        """
        @return: A dict contains key/value pairs of cookies, which contains
                 bytes instead of unicode.
        """

        raw_cookies = cls._get_login_raw_cookies(username, password)
        # extract cookies and return.
        cookies_dict = cls._trans_unicode_element_of_dict(raw_cookies)
        return cookies_dict

    @classmethod
    def get_login_cookies_jar(cls, username, password):
        """
        @return: A CookieJar object.
        """

        raw_cookies = cls._get_login_raw_cookies(username, password)
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
class ColaLogin(object):

    def __init__(self, username, passwd):
        self.opener = PageLoader(cookielib.CookieJar())

        self.username = username
        self.passwd = passwd

    def get_user(self, username):
        username = urllib.quote(username)
        return base64.encodestring(username)[:-1]

    def get_passwd(self, passwd, pubkey, servertime, nonce):
        key = rsa.PublicKey(int(pubkey, 16), int('10001', 16))
        message = str(servertime) + b'\t' + str(nonce) + b'\n' + str(passwd)
        passwd = rsa.encrypt(message, key)
        return binascii.b2a_hex(passwd)

    def prelogin(self):
        username = self.get_user(self.username)
        prelogin_url = (
            'http://login.sina.com.cn/sso/prelogin.php?'
            'entry=sso&callback=sinaSSOController.preloginCallBack&'
            'su={}&rsakt=mod&client=ssologin.js(v1.4.5)'
        ).format(username)
        data = self.opener.open(prelogin_url)
        regex = re.compile('\((.*)\)')
        try:
            json_data = regex.search(data).group(1)
            data = json.loads(json_data)

            return (
                str(data['servertime']),
                data['nonce'].encode("utf-8"),
                data['pubkey'].encode("utf-8"),
                data['rsakv'].encode("utf-8"),
            )
        except:
            raise Exception("WeiboLoginFailure")

    def login(self):
        login_url = (
            'http://login.sina.com.cn/sso/login.php?'
            'client=ssologin.js(v1.4.5)'
        )

        try:
            servertime, nonce, pubkey, rsakv = self.prelogin()
            postdata = {
                'entry': b'weibo',
                'gateway': b'1',
                'from': b'',
                'savestate': b'7',
                'userticket': b'1',
                'ssosimplelogin': b'1',
                'vsnf': b'1',
                'vsnval': '',
                'su': self.get_user(self.username),
                'service': b'miniblog',
                'servertime': servertime,
                'nonce': nonce,
                'pwencode': b'rsa2',
                'sp': self.get_passwd(
                    self.passwd,
                    pubkey,
                    servertime,
                    nonce),
                'encoding': b'UTF-8',
                'prelt': b'115',
                'rsakv': rsakv,
                'url': (
                    b'http://weibo.com/ajaxlogin.php?framelogin=1&amp;'
                    b'callback=parent.sinaSSOController.feedBackUrlCallBack'
                ),
                'returntype': b'META'}
            postdata = urllib.urlencode(postdata)
            text = self.opener.open(login_url, postdata)

            # Fix for new login changed since about 2014-3-28
            ajax_url_regex = re.compile('location\.replace\(\'(.*)\'\)')
            matches = ajax_url_regex.search(text)
            if matches is not None:
                ajax_url = matches.group(1)
                text = self.opener.open(ajax_url)

            regex = re.compile('\((.*)\)')
            json_data = json.loads(regex.search(text).group(1))
            result = json_data['result']
            if result is False:
                # return result, json_data['reason']
                return None
            return result
        except:
            return None


class NonGUIBasedLogin(object):

    @classmethod
    def get_login_cookies_jar(cls, username, password):
        """
        @input: username and password should all be unicode.
        """
        username = username.encode("utf-8")
        password = password.encode("utf-8")
        cola_login_handler = ColaLogin(username, password)
        if cola_login_handler.login():
            return cola_login_handler.opener.cj
        else:
            return None


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
