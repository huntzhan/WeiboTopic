
from easy_spider import ElementProcessor
from .utils import LoginHandler


class UrlProcessor(ElementProcessor):

    def _get_login_cookies(self):
        self._cookies_after_login = LoginHandler.login_and_get_cookies()

    def _check_login_url(self, url):
        return LoginHandler.check_login_url(url)
