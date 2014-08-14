# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import time
import logging
from logging.handlers import SMTPHandler
import re

from sqlalchemy import create_engine, MetaData, func

from .persist import DB_URL


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
    def _extract_key(cls, db_name):
        name = re.sub(r'\d+', '', db_name)
        match = re.search(r'\d+', db_name)
        if match:
            return match.group(0), name
        else:
            return '', ''

    @classmethod
    def report(cls):
        engine = create_engine(DB_URL)
        metadata = MetaData()
        metadata.reflect(engine)

        texts = []
        for table_name in sorted(metadata.tables,
                                 key=cls._extract_key):
            table = metadata.tables[table_name]
            size = engine.execute(
                select([func.count()]).select_from(table),
            ).scalar()
            # format
            epoch, _ = cls._extract_key(table_name)
            if epoch:
                begin = time.mktime(int(epoch))
                end = time.mktime(int(epoch) + 3600)
                time_format = '%m/%d/%H'
                duration = "[{0} - {1}]".format(
                    time.strftime(time_format, begin),
                    time.strftime(time_format, end),
                )
                table_name += duration
            else:
                table_name += '[unknow]'
            texts.append(
                "{0}: {1}".format(table_name, size))

        text = '\n'.join(texts)
        logger.info(text.encode('utf-8'))
