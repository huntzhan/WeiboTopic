from __future__ import (unicode_literals, print_function, absolute_import)

import time
import re
import cookielib

from selenium import webdriver
from requests.cookies import create_cookie


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
