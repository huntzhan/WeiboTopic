# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import time
import logging
from logging.handlers import SMTPHandler

from sqlalchemy import func

from .persist import _WeiboUser, _User2Blog, _Microblog, DatabaseHandler


def _get_size(pri_key):
    session = DatabaseHandler.Session()
    size = session.query(func.count(pri_key)).one()[0]
    session.close()
    return size


def get_WeiboUser():
    return _get_size(_WeiboUser.uid)


def get_User2Blog():
    return _get_size(_User2Blog.uid)


def get_Microblog():
    return _get_size(_Microblog.mid)


class ProjectSMTPHandler(SMTPHandler):

    email = b'huntzhan@tencent.com'

    def getSubject(self, recort):
        return b'[虚拟项目] 数据监控 ' + time.ctime()


formatter = logging.Formatter(b'%(message)s\n')
stream_handler = logging.StreamHandler()
stream_handler.setFormatter(formatter)

email_handler = ProjectSMTPHandler(
    b'smtp.163.com',
    b'vproject_logging@163.com',
    ProjectSMTPHandler.email,
    b'',
    (b'vproject_logging@163.com', b'tencent'),
)
email_handler.setFormatter(formatter)

logger = logging.getLogger(__name__)
logger.addHandler(stream_handler)
logger.addHandler(email_handler)
logger.setLevel(logging.INFO)


MESSAGE_TEMPLATE = (
    b"Time: {0} - {1}"
    b"New: WeiboUser: {2}; MicroBlog: {3}; User2Blog: {4}"
    b"Current: WeiboUser: {5}; MicroBlog: {6}; User2Blog: {7}"
)


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
            time.asctime(one_hour_ago),
            time.asctime(current_time),
            diffs[0],
            diffs[1],
            diffs[2],
            cls.size_of_weibouser,
            cls.size_of_microblog,
            cls.size_of_user2blog,
        )
        logger.info(text.encode('utf-8'))
