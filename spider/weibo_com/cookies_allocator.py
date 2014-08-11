
from __future__ import (unicode_literals, print_function, absolute_import)

import re
import random
from collections import defaultdict
from .utils import NonGUIBasedLogin, GUIBasedLogin


class CookiesAllocator(object):
    """
    @brief: 1. Maintains username/password pairs.
            2. Maintains a list of available cookies.
            3. Provides a interface for processors to retrive cookies.
            4. Intergrates two different login strategies.
    """

    # number of cookies per user.
    COOKIES_PER_USER = 3
    # minimum accepted health level of cookies.
    ACCEPT_HEALTH_LEVEL = 0.6

    USERNAME_PASSWORD_MAPPING = {
        "janfancoat2@163.com": "coat123456",
        "tencent_vproject1@163.com": "tencent",
        "vproject_janfan1@163.com": "tencent",
        "vproject_janfan2@163.com": "tencent",
        "vproject_janfan3@163.com": "tencent",
    }
    # username -> [cookiejar...]
    _username_cookies_mapping = defaultdict(list)

    @classmethod
    def _check_cookies_availability(cls):
        """
        @brief: health level = current size of cookies / maximum of cookies. If
                health level < 60%, cookies should be refreshed.
        """

        # calculate health level.
        maximum = cls.COOKIES_PER_USER * len(cls.USERNAME_PASSWORD_MAPPING)
        current_cookies = 0
        for cookies in cls._username_cookies_mapping.values():
            current_cookies = current_cookies + len(cookies)
        health_level = float(current_cookies) / maximum

        if health_level >= cls.ACCEPT_HEALTH_LEVEL:
            return True
        return False

    @classmethod
    def _non_gui_based_method(cls, username, password):
        cookies_jar = NonGUIBasedLogin.get_login_cookies_jar(
            username,
            password,
        )
        return cookies_jar

    @classmethod
    def _gui_based_method(cls, username, password):
        cookies_jar = GUIBasedLogin.get_login_cookies_jar(
            username,
            password,
        )
        return cookies_jar

    @classmethod
    def _build_cookies_with_callback(cls, callback):
        for username, password in cls.USERNAME_PASSWORD_MAPPING.items():
            cookies_jars = cls._username_cookies_mapping[username]
            diff = cls.COOKIES_PER_USER - len(cookies_jars)
            for _ in range(diff):
                cookies_jar = callback(username, password)
                if cookies_jar:
                    cookies_jars.append(cookies_jar)

    @classmethod
    def build_cookeis(cls):
        """
        @brief: Refresh cookies. Non-GUI based method would be applied first.
                If cookies are still unavailable, then GUI based method would
                be applied.
        """

        # Notices that _username_cookies_mapping might not be empty.
        # use non-GUI based method first.
        cls._build_cookies_with_callback(cls._non_gui_based_method)
        # check availability of cookies.
        if cls._check_cookies_availability():
            # ok!
            return
        # seems not ok. use GUI based method.
        cls._build_cookies_with_callback(cls._gui_based_method)

    @classmethod
    def check_login_url(cls, url):
        """
        @return: True if url points to login page.
        """

        LOGIN_URL_PATTERNS = [
            "weibo.com/login",
            "login.sina.com.cn",
            "weibo.com/signup",
            "passport.weibo.com",
        ]
        for pattern in LOGIN_URL_PATTERNS:
            if re.search(pattern, url):
                return True
        return False

    @classmethod
    def get_cookies_jar(cls):
        """
        @brief: random select cookies.
        """

        if not cls._check_cookies_availability():
            # refresh cookies.
            cls.build_cookeis()

        cookies_jars = random.choice(
            # filter out empty lists.
            filter(bool, cls._username_cookies_mapping.values()),
        )
        return random.choice(cookies_jars)

    @classmethod
    def set_cookies_jar_invalid(cls, cookies_jar):
        for cookies_jars in cls._username_cookies_mapping.values():
            if cookies_jar in cookies_jars:
                cookies_jars.remove(cookies_jar)
