from __future__ import (unicode_literals, print_function, absolute_import)

import time
import re
import cookielib
import urllib
import json

from selenium import webdriver
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
        keys = raw_input("Enter vertification code> ")
        vertify_ele.clear()
        vertify_ele.send_keys(keys)

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
    USERNAME = "13570322748"
    PASSWORD = "huntzhan"

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
    def _get_login_driver(cls):
        """
        @brief: Login base on GUI, with consideration of vetification.
        @return: A browser driver was successfully logined.
        """

        driver = webdriver.Firefox()
        driver.get(cls.LOGIN_URL)
        # try to login without vetification.
        cls._fill_username_and_password(driver)
        cls._submit(driver)

        while True:
            # wait for page load.
            time.sleep(0.3)
            try:
                cls._fill_username_and_password(driver)
                cls._fill_vertification(driver)
                cls._submit(driver)
            except:
                break
        # After login sina.com, we need to get login session from weibo.com.
        driver.get("http://weibo.com/")
        return driver

    @classmethod
    def get_login_cookies_dict(cls):
        """
        @return: A dict contains key/value pairs of cookies, which contains
                 bytes instead of unicode.
        """

        driver = cls._get_login_driver()
        # extract cookies and return.
        cookies_dict = cls._trans_unicode_element_of_dict(
            driver.get_cookies())
        driver.close()
        return cookies_dict

    @classmethod
    def get_login_cookies_jar(cls):
        """
        @return: A CookieJar object.
        """

        driver = cls._get_login_driver()
        cookiejar = cls._trans_cookies_from_dict_to_cookiejar(
            driver.get_cookies())
        driver.close()
        return cookiejar


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


def beautiful_soup(html, logger=None):
    try:
        from bs4 import BeautifulSoup, FeatureNotFound
    except ImportError:
        raise Exception("BeautifulSoup4")
    try:
        return BeautifulSoup(html, 'lxml')
    except FeatureNotFound:
        return BeautifulSoup(html)


def urldecode(link):
    decodes = {}
    if '?' in link:
        params = link.split('?')[1]
        for param in params.split('&'):
            k, v = tuple(param.split('='))
            decodes[k] = urllib.unquote(v)
    return decodes


class FansPageParser(object):

    def parse(self, requested_url, response_content):
        """
        @input: requested url, content page being loaded.
        @output: is_follow, True if requested_url points to followers, False if
                 requested_url points fans; new_uids, list of new uids.
        """
        variables = self._gen_internal_variables(requested_url, response_content)
        if variables is None:
            return None
        else:
            html, decodes, is_follow, is_new_mode = variables

        new_uids = self._extract_uids(html, decodes)
        return new_uids

    ###############
    # refactoring #
    ###############
    def _gen_internal_variables(self, requested_url, response_content):
        """
        @return: html, decodes, is_follow, is_new_mode.
        """
        # if self.bundle.exists == False:
        #     return [], []

        # url = url or self.url

        # br, soup = None, None
        # try:
        #     br = self.opener.browse_open(url)
        #     self.logger.debug('load %s finish' % url)
        #     soup = beautiful_soup(br.response().read())
        # except Exception as e:
        #     return self._error(url, e)

        # if not self.check(url, br):
        #     return [], []

        # weibo_user = self.get_weibo_user()
        soup = beautiful_soup(response_content)

        html = None
        decodes = urldecode(requested_url)
        is_follow = True
        is_new_mode = False
        for script in soup.find_all('script'):
            text = script.text
            if text.startswith('FM.view'):
                text = text.strip().replace(
                    ';', '').replace(
                    'FM.view(', '')[
                    :-1]
                data = None
                try:
                    data = json.loads(text)
                except ValueError as e:
                    # return self._error(url, e)
                    return None

                domid = data['domid']
                if domid.startswith('Pl_Official_LeftHisRelation__'):
                    html = beautiful_soup(data['html'])
                if 'relate' in decodes and decodes['relate'] == 'fans':
                    is_follow = False
                is_new_mode = True
            elif 'STK' in text:
                text = text.replace(
                    'STK && STK.pageletM && STK.pageletM.view(', '')[
                    :-1]
                data = json.loads(text)
                if data['pid'] == 'pl_relation_hisFollow' or \
                        data['pid'] == 'pl_relation_hisFans':
                    html = beautiful_soup(data['html'])
                if data['pid'] == 'pl_relation_hisFans':
                    is_follow = False

        return html, decodes, is_follow, is_new_mode

    ###############
    # refactoring #
    ###############
    def _extract_uids(self, html, decodes):
        """
        @return: new uids.
        """
        new_uids = []

        ul = None
        try:
            ul = html.find(
                attrs={
                    'class': 'cnfList',
                    'node-type': 'userListBox'})
        except AttributeError as e:
            # if br.geturl().startswith('http://e.weibo.com'):
            #     return [], []
            # return self._error(url, e)
            return None
        if ul is None:
            # urls = []
            # if is_follow is True:
            #     if is_new_mode:
            #         urls.append(
            #             'http://weibo.com/%s/follow?relate=fans' %
            #             self.uid)
            #     else:
            #         urls.append('http://weibo.com/%s/fans' % self.uid)
            return None

        # current_page = decodes.get('page', 1)
        # if current_page == 1:
        #     if is_follow:
        #         weibo_user.follows = []
        #     else:
        #         weibo_user.fans = []
        for li in ul.find_all(attrs={'class': 'S_line1',
                                     'action-type': 'itemClick'}):
            data = dict([l.split('=') for l in li['action-data'].split('&')])

            # friend = Friend()
            # friend.uid = data['uid']
            # friend.nickname = data['fnick']
            # friend.sex = True if data['sex'] == u'm' else False

            # bundles.append(WeiboUserBundle(str(friend.uid)))
            # if is_follow:
            #     weibo_user.follows.append(friend)
            # else:
            #     weibo_user.fans.append(friend)

            new_uids.append(data['uid'])

        # weibo_user.save()
        # self.logger.debug('parse %s finish' % url)
        return new_uids

    ###############
    # refactoring #
    ###############
    def _gen_next_page(self, html, decodes):
        pages = html.find(
            'div',
            attrs={
                'class': 'W_pages',
                'node-type': 'pageList'})
        if pages is not None:
            a = pages.find_all('a')
            if len(a) > 0:
                next_ = a[-1]
                if next_['class'] == ['W_btn_c']:
                    decodes['page'] = int(decodes.get('page', 1)) + 1
                    query_str = urllib.urlencode(decodes)
                    url = '%s?%s' % (url.split('?')[0], query_str)

                    # return urls, bundles
                    return url
        return None

        # if is_follow is True:
        #     if is_new_mode:
        #         urls.append(
        #             'http://weibo.com/%s/follow?relate=fans' %
        #             self.uid)
        #     else:
        #         urls.append('http://weibo.com/%s/fans' % self.uid)
        #
        # return urls, bundles

    def _gen_fans_page(self, uid):
        pass
