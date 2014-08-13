# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import time
from logging.handlers import SMTPHandler

from .persist import _WeiboUser, _User2Blog, _Microblog, DatabaseHandler


def _get_size(model):
    session = DatabaseHandler.Session()
    size = session.query(model).count()
    session.close()
    return size


def get_WeiboUser():
    return _get_size(_WeiboUser)


def get_User2Blog():
    return _get_size(_User2Blog)


def get_Microblog():
    return _get_size(_Microblog)


class ProjectSMTPHandler(SMTPHandler):

    email = 'huntzhan@tencent.com'

    def getSubject(self, recort):
        return '[虚拟项目] 数据监控 ' + time.ctime()


formatter = logging.Formatter('%(message)s\n')
stream_handler = logging.StreamHandler()
stream_handler.setFormatter(formatter)

email_handler = ProjectSMTPHandler(
    'smtp.163.com',
    'vproject_logging@163.com',
    ProjectSMTPHandler.email,
    '',
    ('vproject_logging@163.com', 'tencent'),
)
email_handler.setFormatter(formatter)

logger = logging.getLogger(__name__)
logger.addHandler(stream_handler)
logger.addHandler(email_handler)
logger.setLevel(logging.INFO)


MESSAGE_TEMPLATE = """
Time: {0} - {1}
New: WeiboUser: {2}; MicroBlog: {3}; User2Blog: {4}
Current: WeiboUser: {5}; MicroBlog: {6}; User2Blog: {7}
"""


class Statistics(object):
    size_of_weibouser = None
    size_of_user2blog = None
    size_of_microblog = None

    @classmethod
    def loadup(cls):
        cls.size_of_weibouser = get_WeiboUser()
        cls.size_of_microblog = get_Microblog()
        cls.size_of_user2blog = get_User2Blog()

    @classmethod
    def update_and_get_diff(cls):
        diff_weibouser = get_WeiboUser() - cls.size_of_weibouser
        diff_microblog = get_Microblog() - cls.size_of_microblog
        diff_user2blog = get_User2Blog() - cls.size_of_user2blog
        cls.loadup()
        return diff_weibouser, diff_microblog, diff_user2blog

    @classmethod
    def report(cls):
        diffs = cls.update_and_get_diff()
        current_time = time.localtime(time.time())
        one_hour_ago = time.localtime(time.time() - 3600)

        text = MESSAGE_TEMPLATE.format(
            one_hour_ago.asctime(),
            current_time.asctime(),
            diffs[0],
            diffs[1],
            diffs[2],
            cls.size_of_weibouser,
            cls.size_of_microblog,
            cls.size_of_user2blog,
        )
        logger.info(text)
