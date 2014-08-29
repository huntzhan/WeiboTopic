from __future__ import (unicode_literals, print_function, absolute_import)

import logging
import subprocess
import sys
import time

from .bussiness import Schedule, WeiboAPIHandler, PublicTimelineQuery
from .config import ConfigurationCenter
from .logger import setup_logging
from .statistics import Statistics


QUERY_PER_HOUT = 145
DURATION = 3600.0 / QUERY_PER_HOUT


ordered_keys_for_init = [
    'username', 'password',
    'app_id', 'app_secret',
    'code', 'access_token',
    'redirect_uri',
]


def api():
    setup_logging()
    logger = logging.getLogger()

    ConfigurationCenter.load_configuration()
    logger.info("Finished prepareing.")

    while True:
        # for each keys, init handler and query.
        queries = []
        for info in ConfigurationCenter.generate_app_infos():
            args = [info[key] for key in ordered_keys_for_init]
            handler = WeiboAPIHandler(*args)
            query = PublicTimelineQuery(handler)
            queries.append(query)
        logger.info("Init %s queries", len(queries))

        # timming.
        schedule = Schedule(DURATION)
        for query in queries:
            schedule.add_callback(query.query)

        try:
            schedule.run()
        except Exception as e:
            logger.warning(e)
            time.sleep(120)

        logger.info("Finished loop.")


def report():
    HOURS = 24
    ONE_DAY = 3600.0 * HOURS

    Statistics.report()
    while True:
        schedule = Schedule(ONE_DAY)
        for _ in range(HOURS):
            schedule.add_callback(Statistics.report)
        schedule.run()
