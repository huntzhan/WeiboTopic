from __future__ import (unicode_literals, print_function, absolute_import)

import time

from weibo_com.storage import WeiboUser


class user:
    pass


class Url(object):
    def __init__(self, url, force=False):
        self.url = url
        self.force = force

    def __str__(self):
        return self.url


class Bundle(object):
    '''
    Sometimes the target is all the urls about a user.
    Then the urls compose the bundle.
    So a bundle can generate several urls.
    '''

    def __init__(self, label, force=False):
        if not isinstance(label, str):
            raise ValueError("Bundle's label must a string.")
        self.label = label
        self.force = force

    def urls(self):
        raise NotImplementedError

    def __str__(self):
        return self.label


class WeiboUserBundle(Bundle):
    def __init__(self, uid):
        super(WeiboUserBundle, self).__init__(uid)
        self.uid = uid
        self.exists = True

        self.last_error_page = None
        self.last_error_page_times = 0

        self.weibo_user = None
        self.last_update = None
        self.newest_mids = []
        self.current_mblog = None

    def urls(self):
        start = int(time.time() * (10**6))
        return [
            'http://weibo.com/%s/follow' % self.uid
            # 'http://weibo.com/aj/mblog/mbloglist?uid=%s&_k=%s' %
            # (self.uid, start)
            # 'http://weibo.com/%s/info' % self.uid,
            # remove because some user's link has been
            # http://weibo.com/uid/follow?relate=fans
            # 'http://weibo.com/%s/fans' % self.uid
        ]