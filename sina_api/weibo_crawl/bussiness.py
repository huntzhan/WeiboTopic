from __future__ import (unicode_literals, print_function, absolute_import)

from collections import namedtuple
import logging
from threading import Timer
import time
import re
import urllib
import urlparse

import requests
from selenium import webdriver

from .config import ConfigurationCenter
from .persist import WeiboUserHandler, MicroblogHandler, DatabaseHandler


logger = logging.getLogger(__name__)


STOP = []


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
        logger.info("Setup tasks.")

        # start timer.
        for timer in tasks:
            timer.start()
        # just wait.
        logger.info("Sleep")
        time.sleep(self.duration)
        # wait for authorization.
        while STOP:
            pass


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
            # remove it since its uncontrollable.
            # try:
            #     # still in login page.
            #     sop.fill_username_and_password(
            #         driver,
            #         username, password,
            #     )
            #     sop.fill_vertification(driver)
            #     sop.submit(driver)
            # except:
            #     pass
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
        # simple mutex with GIL.
        STOP.append(True)
        target_url = cls._gui_operation(
            client_id, redirect_uri,
            username, password,
        )
        STOP.pop(0)
        return cls._extract_code(target_url)


class WeiboAPIHandler(object):

    ACCESS_URL = "https://api.weibo.com/oauth2/access_token"

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

    messages_names_mapping = {
        'mid': 'mid',
        'content': 'text',
        # 'forwarded_content': None,
        'created_time': 'created_at',
        'favorites': 'attitudes_count',
        'comments': 'comments_count',
        'forwards': 'reposts_count',
        'source': 'source',
    }

    user_names_mapping = {
        'uid': 'id',
        'fans': 'followers_count',
        'followees': 'friends_count',
        'posts': 'statuses_count',
        'sex': 'gender',
        'favourites_count': 'favourites_count',
        'created_at': 'created_at',
        'verified': 'verified',
        'bi_followers_count': 'bi_followers_count',
    }

    Message = namedtuple(
        'Message',
        messages_names_mapping.keys(),
    )

    User = namedtuple(
        'User',
        user_names_mapping.keys(),
    )

    def __init__(self, api_handler):
        self.api_handler = api_handler

    def _build_data_interface(self, item, names_mapping, interface_cls):
        # regular expression for filter out 4-byte char.
        try:
            highpoints = re.compile('[\U00010000-\U0010ffff]')
        except re.error:
            # UCS-2 build
            highpoints = re.compile('[\uD800-\uDBFF][\uDC00-\uDFFF]')

        kwargs = {}
        for custom_key, api_key in names_mapping.items():
            value = item.get(api_key, None)
            # filter out 4-byte char.
            if isinstance(value, unicode):
                value = highpoints.sub('', value)
            kwargs[custom_key] = value
        return interface_cls(**kwargs)

    def _extract_message(self, item):
        message = self._build_data_interface(
            item,
            self.messages_names_mapping,
            self.Message,
        )
        # post process.
        extracted_source = re.search(r"<.+?>(.*?)<.+?>", message.source)
        # generate new message.
        data = dict(message._asdict())
        data['source'] = extracted_source.group(1)\
            if extracted_source else None
        message = self.Message(**data)
        return message

    def _extract_user(self, item):
        user_object = item['user']
        user = self._build_data_interface(
            user_object,
            self.user_names_mapping,
            self.User,
        )
        return user

    def query(self):
        logger.info("Getting messages.")
        response_json = self.api_handler.apply(
            self.URL,
            count=200,
        )
        logger.info("Got messages.")

        items = response_json['statuses']
        for item in items:
            user = self._extract_user(item)
            message = self._extract_message(item)
            # examine timestamp of msg
            # t_str = message['created_time']
            # t_struct = time.strptime(t_str, "%a %b %d %H:%M:%S +0800 %Y")
            # t_sec = time.mktime(t_struct)
            # if t_sec-DatabaseHandler.NOW_IN_HOUR > 3600:
            #     DatabaseHandler.switch_tables()

            logger.debug("Adding user to db.")
            WeiboUserHandler.add_user(
                **dict(user._asdict())
            )
            logger.debug("Finished adding user to db.")

            logger.debug("Adding message to db.")
            MicroblogHandler.add_blog(
                uid=user.uid,
                **dict(message._asdict())
            )
            logger.debug("Finished Adding message to db.")

        logger.info("Finished query.")
