#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import urllib

from weibo_com.error import LoginFailure
from weibo_com.weibo_user import WeiboUserBundle
from weibo_com.beautiful_soup import beautiful_soup


TIMEOUT = 30.0


def urldecode(link):
    decodes = {}
    if '?' in link:
        params = link.split('?')[1]
        for param in params.split('&'):
            k, v = tuple(param.split('='))
            decodes[k] = urllib.unquote(v)
    return decodes


class Parser(object):
    def __init__(self, opener=None, url=None, **kwargs):
        self.opener = opener
        if url is not None:
            self.url = url
        for k, v in kwargs.iteritems():
            setattr(self, k, v)

    def parse(self, url=None):
        raise NotImplementedError


class WeiboParser(Parser):
    def __init__(self, opener=None, url=None, bundle=None, **kwargs):
        super(WeiboParser, self).__init__(opener=opener, url=url, **kwargs)
        self.bundle = bundle
        self.uid = bundle.label
        self.opener.set_default_timeout(TIMEOUT)

    def _check_url(self, dest_url, src_url):
        return dest_url.split('?')[0] == src_url.split('?')[0]

    def check(self, url, br):
        dest_url = br.geturl()
        if not self._check_url(dest_url, url):
            if dest_url.startswith('http://weibo.com/login.php'):
                raise LoginFailure('Weibo not login or login expired')
            if dest_url.startswith('http://weibo.com/sorry?usernotexists'):
                self.bundle.exists = False
                return False
        return True

    # def get_weibo_user(self):
        # if self.bundle.weibo_user is not None:
        #     return self.bundle.weibo_user

        # try:
        #     self.bundle.weibo_user = getattr(WeiboUser, 'objects').
        #         get(uid=self.uid)
        # except DoesNotExist:
        #     self.bundle.weibo_user = WeiboUser(uid=self.uid)
        #     self.bundle.weibo_user.save()
        # return self.bundle.weibo_user

    def _error(self, url, e):
        if self.bundle.last_error_page == url:
            self.bundle.last_error_page_times += 1
        else:
            self.bundle.last_error_page = url
            self.bundle.last_error_page_times = 0

        if self.bundle.last_error_page_times >= 15:
            raise e
        return [url, ], []


class UserFriendParser(WeiboParser):
    def parse(self, url=None):
        if not self.bundle.exists:
            return [], []

        url = url or self.url

        br, soup = None, None
        try:
            br = self.opener.browse_open(url)
            # self.logger.debug('load %s finish' % url)
            soup = beautiful_soup(br.response().read())
        except Exception, e:
            return self._error(url, e)

        if not self.check(url, br):
            return [], []

        # weibo_user = self.get_weibo_user()

        html = None
        decodes = urldecode(url)
        is_follow = True
        is_new_mode = False
        for script in soup.find_all('script'):
            text = script.text
            if text.startswith('FM.view'):
                text = text.strip().replace(';', ''). \
                    replace('FM.view(', '')[:-1]
                data = None
                try:
                    data = json.loads(text)
                except ValueError, e:
                    return self._error(url, e)
                domid = data['domid']
                if domid.startswith('Pl_Official_LeftHisRelation__'):
                    html = beautiful_soup(data['html'])
                if 'relate' in decodes and decodes['relate'] == 'fans':
                    is_follow = False
                is_new_mode = True
            elif 'STK' in text:
                text = text.replace(
                    'STK && STK.pageletM && STK.pageletM.view(',
                    '')[:-1]
                data = json.loads(text)
                if data['pid'] == 'pl_relation_hisFollow' or \
                        data['pid'] == 'pl_relation_hisFans':
                    html = beautiful_soup(data['html'])
                if data['pid'] == 'pl_relation_hisFans':
                    is_follow = False

        bundles = []
        ul = None
        try:
            ul = html.find(attrs={
                'class': 'cnfList',
                'node-type': 'userListBox'})
        except AttributeError, e:
            if br.geturl().startswith('http://e.weibo.com'):
                return [], []
            return self._error(url, e)
        if ul is None:
            urls = []
            if is_follow is True:
                if is_new_mode:
                    urls.append('http://weibo.com/%s/follow?relate=fans' %
                                self.uid)
                else:
                    urls.append('http://weibo.com/%s/fans' % self.uid)
            return urls, bundles

        # current_page = decodes.get('page', 1)
        # if current_page == 1:
            # if is_follow:
            #     weibo_user.follows = []
            # else:
            #     weibo_user.fans = []
        for li in ul.find_all(attrs={
                'class': 'S_line1',
                'action-type': 'itemClick'}):
            data = dict([l.split('=') for l in li['action-data'].split('&')])

            # friend = Friend()
            # friend.uid = data['uid']
            # friend.nickname = data['fnick']
            # friend.sex = True if data['sex'] == u'm' else False

            # bundles.append(WeiboUserBundle(str(friend.uid)))
            bundles.append(WeiboUserBundle(str(data['uid'])))
            # if is_follow:
            #     weibo_user.follows.append(friend)
            # else:
            #     weibo_user.fans.append(friend)

        # weibo_user.save()
        # self.logger.debug('parse %s finish' % url)

        urls = []
        pages = html.find('div', attrs={
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
                    urls.append(url)

                    return urls, bundles

        if is_follow is True:
            if is_new_mode:
                urls.append('http://weibo.com/%s/follow?relate=fans' % self.uid)
            else:
                urls.append('http://weibo.com/%s/fans' % self.uid)

        return urls, bundles
