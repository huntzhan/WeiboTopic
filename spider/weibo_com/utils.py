from __future__ import (unicode_literals, print_function, absolute_import)

import re
from selenium import webdriver


class LoginHandler(object):
    """
    @brief: Login to weibo.cn and return cookies.
    """

    LOGIN_URL = "http://login.sina.com.cn/signup/signin.php?entry=sso"
    # Currently set username and password as class variable, should be changed
    # when simulating multiple users.
    USERNAME = "13570322748"
    PASSWORD = "huntzhan"

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
        LOGIN_URL_PATTERNS = [
            "weibo.com/login",
            "login.sina.com.cn",
        ]
        for pattern in LOGIN_URL_PATTERNS:
            if re.search(pattern, url):
                return True
        return False

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
    def login_and_get_cookies(cls):
        """
        @brief: Login base on GUI, with consideration of vetification.
        @return: A dict contains key/value pairs of cookies, which contains
                 bytes instead of unicode.
        """

        driver = webdriver.Firefox()
        driver.get(cls.LOGIN_URL)
        # try to login without vetification.
        cls._fill_username_and_password(driver)
        cls._submit(driver)

        while True:
            try:
                cls._fill_username_and_password(driver)
                cls._fill_vertification(driver)
                cls._submit(driver)
            except:
                break

        # extract cookies and return.
        cookies_dict = cls._trans_unicode_element_of_dict(
            driver.get_cookies())
        driver.close()
        return cookies_dict
