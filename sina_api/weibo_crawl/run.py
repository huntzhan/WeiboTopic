from __future__ import (unicode_literals, print_function, absolute_import)

import logging

from .bussiness import Schedule, WeiboAPIHandler, PublicTimelineQuery
from .config import ConfigurationCenter
from .persist import DatabaseHandler
from .logger import setup_logging


setup_logging()
logger = logging.getLogger()


QUERY_PER_HOUT = 145
DURATION = 3600.0 / QUERY_PER_HOUT


ordered_keys_for_init = [
    'username', 'password',
    'app_id', 'app_secret',
    'code', 'access_token',
    'redirect_uri',
]


def run():
    ConfigurationCenter.load_configuration()
    DatabaseHandler.open()
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
        schedule.run()
        logger.info("Finished loop.")
