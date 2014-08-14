# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import time
import logging
from logging.handlers import SMTPHandler

from sqlalchemy import func

from .model_manager import ModelManager


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


class Statistics(object):

    @classmethod
    def loadup(cls):
        pass

    @classmethod
    def update_and_get_diff(cls):
        cls.loadup()

    @classmethod
    def report(cls):
        diffs = cls.update_and_get_diff()
        current_time = time.localtime(time.time())
        one_hour_ago = time.localtime(time.time() - 3600)

        text = b''
        logger.info(text.encode('utf-8'))
