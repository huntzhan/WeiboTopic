from __future__ import (unicode_literals, print_function, absolute_import)

import re
from selenium import webdriver
from easy_spider import ElementProcessor


class LoginHandler(object):
    """
    @brief: Login to weibo.cn and return cookies.
    """

    LOGIN_URL = "http://login.weibo.cn/login/"
    USERNAME = "13570322748"
    PASSWORD = "huntzhan"

    @classmethod
    def _fill_username_and_passwd(cls, driver):
        user_ele = driver.find_element_by_xpath("//input[@type='text']")
        user_ele.send_keys(cls.USERNAME)
        passwd_ele = driver.find_element_by_xpath("//input[@type='password']")
        passwd_ele.send_keys(cls.PASSWORD)

    @classmethod
    def _fill_vertification(cls, driver):
        vertify_ele = driver.find_element_by_xpath("//input[@name='code']")
        keys = raw_input("vertification> ")
        vertify_ele.send_keys(keys)

    @classmethod
    def _submit(cls, driver):
        button = driver.find_element_by_xpath("//input[@type='submit']")
        button.click()

    @classmethod
    def _trans_unicode_element_of_dict(cls, source_dicts):
        trans = lambda x: x.encode("utf-8")
        target_dict = {}
        for temp_dict in source_dicts:
            key = temp_dict['name']
            value = temp_dict['value']
            if isinstance(key, unicode):
                key = trans(key)
            if isinstance(value, unicode):
                value = trans(value)
            target_dict[key] = value
        return target_dict

    @classmethod
    def check_login_url(cls, url):
        """
        @return: True if url points to login page.
        """
        if re.search(cls.LOGIN_URL, url):
            return True
        return False

    @classmethod
    def login_and_get_cookies(cls):
        """
        @brief: Login base on GUI, with consideration of vetification.
        @return: A dict contains key/value pairs of cookies.
        """

        driver = webdriver.Firefox()
        driver.get(cls.LOGIN_URL)
        # try to login without vetification.
        cls._fill_username_and_passwd(driver)
        cls._submit(driver)

        if cls.check_login_url(driver.current_url):
            # need vertification code.
            cls._fill_username_and_passwd(driver)
            cls._fill_vertification(driver)
            cls._submit(driver)

        if cls.check_login_url(driver.current_url):
            raise NotImplementedError("What the fuck.")

        # extract cookies and return.
        cookies_dict = cls._trans_unicode_element_of_dict(
            driver.get_cookies())
        driver.close()
        return cookies_dict
