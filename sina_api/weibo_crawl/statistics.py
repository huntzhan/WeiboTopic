# -*- coding: utf-8 -*-
from __future__ import (unicode_literals, print_function, absolute_import)

import time
import logging
from logging.handlers import SMTPHandler
import re
from collections import OrderedDict

from sqlalchemy import create_engine, MetaData, func, select

from .persist import DB_URL


class ProjectSMTPHandler(SMTPHandler):

    email = b'huntzhan@tencent.com'

    def getSubject(self, recort):
        return b'[虚拟项目] 数据监控 ' + time.ctime()


formatter = logging.Formatter(b'%(message)s')
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


class TableState(object):

    def __init__(self, db_url):
        self.db_url = db_url
        self.cached_static_table_names = OrderedDict()

    def extract_key(self, db_name):
        name = re.sub(r'\d+', '', db_name)
        match = re.search(r'\d+', db_name)
        if match:
            return match.group(0), name
        else:
            return '', ''

    def _double_check_strategy(self):
        engine = create_engine(self.db_url)
        metadata = MetaData()
        metadata.reflect(engine)

        def get_table_size(table_name):
            table = metadata.tables[table_name]
            size = engine.execute(
                select([func.count()]).select_from(table),
            ).scalar()
            return size

        table_names = sorted(metadata.tables, key=self.extract_key)
        new_tables = {}
        # detect new tables.
        for table_name in table_names:
            if table_name in self.cached_static_table_names:
                continue
            new_tables[table_name] = get_table_size(table_name)
        # sleep for a while.
        time.sleep(5)
        # detect stable tables.
        for table_name, size in new_tables.items():
            current_size = get_table_size(table_name)
            if current_size != size:
                # size change, not stable.
                pass
            else:
                self.cached_static_table_names[table_name] = current_size

    def get_static_tables(self):
        self._double_check_strategy()
        return self.cached_static_table_names


class Statistics(object):

    table_state = TableState(DB_URL)

    @classmethod
    def report(cls):
        static_tables = cls.table_state.get_static_tables()
        texts = []
        for table_name, size in static_tables.items():
            # format
            LINE_FORMAT = 'Table: {0}, Duration: {1}, Size: {2}'
            DURATION_FORMAT = "[{0} - {1}]"
            TIME_FORMAT = '%m/%d:%H'

            epoch, _ = cls.table_state.extract_key(table_name)
            if epoch:
                begin = time.gmtime(int(epoch))
                end = time.gmtime(int(epoch) + 3600)
                duration = DURATION_FORMAT.format(
                    time.strftime(TIME_FORMAT, begin),
                    time.strftime(TIME_FORMAT, end),
                )
            else:
                duration = '[UNKNOW]'
            texts.append(
                LINE_FORMAT.format(table_name, duration, size),
            )

        text = '\n'.join(texts)
        logger.info(text.encode('utf-8'))
