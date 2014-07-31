#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Copyright (c) 2013 Qin Xuye <qin@qinxuye.me>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Created on 2013-5-17

@author: Chine
'''

import urllib2
import cookielib
import gzip

DependencyNotInstalledError = Exception


class Opener(object):

    def open(self, url):
        raise NotImplementedError

    def ungzip(self, fileobj):
        gz = gzip.GzipFile(fileobj=fileobj, mode='rb')
        try:
            return gz.read()
        finally:
            gz.close()


class MechanizeOpener(Opener):

    def __init__(self, cookie_filename=None, user_agent=None, timeout=None):
        try:
            import mechanize
        except ImportError:
            raise DependencyNotInstalledError('mechanize')

        if user_agent is None:
            user_agent = 'Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)'

        self.browser = mechanize.Browser()

        self.cj = cookielib.LWPCookieJar()
        if cookie_filename is not None:
            self.cj.load(cookie_filename)
        self.browser.set_cookiejar(self.cj)
        self.browser.set_handle_equiv(True)
        self.browser.set_handle_gzip(True)
        self.browser.set_handle_redirect(True)
        self.browser.set_handle_referer(True)
        self.browser.set_handle_robots(False)
        self.browser.addheaders = [
            ('User-agent', user_agent)]

        if timeout is None:
            self._default_timout = mechanize._sockettimeout._GLOBAL_DEFAULT_TIMEOUT
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
