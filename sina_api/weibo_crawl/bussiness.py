from __future__ import (unicode_literals, print_function, absolute_import)

from collections import namedtuple
from threading import Timer
import time
import re
import urllib
import urlparse

import requests
from selenium import webdriver

from .config import ConfigurationCenter


class Schedule(object):

    def __init__(self, duration):
        self.callback_argument_queue = []
        self.duration = duration

    def add_callback(self, callback, *args, **kwargs):
        self.callback_argument_queue.append(
            (callback, args, kwargs),
        )

    def run(self):
        wait_time_per_callback =\
            float(self.duration) / len(self.callback_argument_queue)
        current_wait_time = wait_time_per_callback

        tasks = []
        for callback, args, kwargs in self.callback_argument_queue:
            timer = Timer(
                current_wait_time,
                callback,
                args=args,
                kwargs=kwargs,
            )
            tasks.append(timer)
            # update current_wait_time.
            current_wait_time += wait_time_per_callback

        # start timer.
        for timer in tasks:
            timer.start()
        # just wait.
        time.sleep(self.duration)


class SeleniumOperator(object):

    @classmethod
    def _find_username_password_elements(cls, driver):
        user_ele = driver.find_element_by_xpath("//input[@id='userId']")
        passwd_ele = driver.find_element_by_xpath("//input[@id='passwd']")
        return user_ele, passwd_ele

    @classmethod
    def _find_verify_element(cls, driver):
        vertify_ele = driver.find_element_by_xpath(
            "//input[@class='WB_iptxt oauth_form_input oauth_form_code']")
        return vertify_ele

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
    def fill_username_and_password(cls, driver,
                                   username, password):
        user_ele, passwd_ele = cls._find_username_password_elements(driver)
        cls._fill_username_password_elements(
            driver, user_ele, passwd_ele,
            username, password,
        )
        # remove stupid box.
        passwd_ele.click()

    @classmethod
    def fill_vertification(cls, driver):
        vertify_ele = cls._find_verify_element(driver)
        cls._fill_vertify_element(driver, vertify_ele)

    @classmethod
    def submit(cls, driver):
        button = driver.find_element_by_xpath(
            "//a[@class='WB_btn_login formbtn_01']")
        button.click()


class CodeApplier(object):

    AUTHORIZE_URL = "https://api.weibo.com/oauth2/authorize"

    @classmethod
    def _gui_operation(cls, client_id, redirect_uri,
                       username, password):
        arguments = {
            'client_id': client_id,
            'redirect_uri': redirect_uri,
        }
        encoded_arguments = urllib.urlencode(arguments)
        url = "{0}?{1}".format(cls.AUTHORIZE_URL, encoded_arguments)

        sop = SeleniumOperator
        driver = webdriver.Firefox()
        driver.get(url)

        sop.fill_username_and_password(
            driver,
            username, password,
        )
        sop.submit(driver)

        while True:
            # wait for page load.
            pattern = '^{}'.format(redirect_uri)
            if re.search(pattern, driver.current_url):
                break
            try:
                # still in login page.
                sop.fill_username_and_password(
                    driver,
                    username, password,
                )
                sop.fill_vertification(driver)
                sop.submit(driver)
            except:
                pass
        driver.close()
        return driver.current_url

    @classmethod
    def _extract_code(cls, url):
        query_string = urlparse.urlparse(url).query
        code = urlparse.parse_qs(query_string)['code'][0]
        return code

    @classmethod
    def get_code(cls, client_id, redirect_uri,
                 username, password):
        target_url = cls._gui_operation(
            client_id, redirect_uri,
            username, password,
        )
        return cls._extract_code(target_url)


class WeiboAPIHandler(object):

    ACCESS_URL= "https://api.weibo.com/oauth2/access_token"

    def __init__(self,
                 username, password,
                 client_id, client_secret,
                 code, access_token,
                 redirect_uri):
        self.oauth_arguments = {
            'client_id': client_id,
            'client_secret': client_secret,
            'grant_type': 'authorization_code',
            'code': code,
            'redirect_uri': redirect_uri,
        }
        self.username = username
        self.password = password
        self.access_token = access_token

    def _check_error(self, json):
        if "error_code" in json:
            return True
        return False

    def _get_code(self):
        code = CodeApplier.get_code(
            self.oauth_arguments['client_id'],
            self.oauth_arguments['redirect_uri'],
            self.username, self.password,
        )
        return code

    def _apply_for_access_token(self):
        """
        @brief: http://open.weibo.com/wiki/OAuth2/access_token
        """

        response = requests.post(self.ACCESS_URL, params=self.oauth_arguments)
        response_json = response.json()
        if self._check_error(response_json):
            # update code.
            self.oauth_arguments['code'] = self._get_code()
            # retry again.
            response = requests.post(
                self.ACCESS_URL,
                params=self.oauth_arguments,
            )
            response_json = response.json()

        self.access_token = response_json['access_token']
        # inform the configuration handler to update de configuration.
        ConfigurationCenter.update_by_appid(
            app_id=self.oauth_arguments['client_id'],
            code=self.oauth_arguments['code'],
            access_token=self.access_token,
        )

    def apply(self, url, **kwargs):

        def _internal_apply():
            kwargs['access_token'] = self.access_token
            response = requests.get(url, params=kwargs)
            return response.json()

        response_json = _internal_apply()
        if self._check_error(response_json):
            self._apply_for_access_token()
            response_json = _internal_apply()
        return response_json


class PublicTimelineQuery(object):

    URL = "https://api.weibo.com/2/statuses/public_timeline.json"

    Message = namedtuple(
        'Message',
        ['mid', 'content', 'forwarded_content', 'created_time',
         'favourites', 'comments', 'forwards'],
    )

    User = namedtuple(
        'User',
        ['uid', 'fans', 'followees', 'posts'],
    )

    def __init__(self, api_handler):
        self.api_handler = api_handler

    def _build_data_interface(self, item, names_mapping, interface_cls):
        kwargs = {}
        for custom_key, api_key in names_mapping.items():
            kwargs[custom_key] = item.get(api_key, None)
        return interface_cls(**kwargs)

    def _extract_message(self, item):
        names_mapping = {
            'mid': 'mid',
            'content': 'text',
            'forwarded_content': None,
            'created_time': 'created_at',
            'favourites': 'attitudes_count',
            'comments': 'comments_count',
            'forwards': 'reposts_count',
            # '': 'source',
        }
        message = self._build_data_interface(
            item,
            names_mapping,
            self.Message,
        )
        return message

    def _extract_user(self, item):
        user_object = item['user']
        names_mapping = {
            'uid': 'id',
            'fans': 'followers_count',
            'followees': 'friends_count',
            'posts': 'statuses_count',
            # '': 'gender',
            # '': 'favourites_count',
            # '': 'created_at',
            # '': 'verified',
            # '': 'bi_followers_count',
        }
        user = self._build_data_interface(
            user_object,
            names_mapping,
            self.User,
        )
        return user

    def query(self):
        response_json = self.api_handler.apply(
            self.URL,
            count=200,
        )
        items = response_json['statuses']

        for item in items:
            message = self._extract_message(item)
            user = self._extract_user(item)
        print("Run!")
