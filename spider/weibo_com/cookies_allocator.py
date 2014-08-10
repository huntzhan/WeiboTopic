
from __future__ import (unicode_literals, print_function, absolute_import)

from .utils import NonGUIBasedLogin, GUIBasedLogin


class CookiesAllocator(object):
    """
    @brief: 1. Maintains username/password pairs.
            2. Maintains a list of avaliable cookies.
            3. Provides a interface for processors to retrive cookies.
            4. Intergrates two different login strategies.
    """

    # number of cookies per user.
    COOKIES_PER_USER = 3
    # minimum accepted health level of cookies.
    ACCEPT_HEALTH_LEVEL = 0.6

    USERNAME_PASSWORD_MAPPING = {
        "janfancoat2@163.com": "coat123456",
    }
    # username -> [cookiejar...]
    _username_cookies_mapping = {}

    @classmethod
    def _check_cookies_availability(cls):
        """
        @brief: health level = current size of cookies / maximum of cookies. If
                health level < 60%, cookies should be refreshed.
        """

        # calculate health level.
        maximum = cls.COOKIES_PER_USER * len(cls.USERNAME_PASSWORD_MAPPING)
        current_cookies = 0
        for cookies in cls._username_cookies_mapping.keys():
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
    def build_cookeis(cls):
        """
        @brief: Refresh cookies. Non-GUI based method would be applied first.
                If cookies are still unavailable, then GUI based method would
                be applied.
        """
        pass

    @classmethod
    def get_cookie_jar(cls, retry=False):
        pass
