from __future__ import (unicode_literals, print_function, absolute_import)

from .bussiness import Schedule, WeiboAPIHandler, PublicTimelineQuery
from .config import ConfigurationCenter
from .persist import DatabaseHandler


QUERY_PER_HOUT = 145
DURATION = 1800.0 / QUERY_PER_HOUT


ordered_keys_for_init = [
    'username', 'password',
    'app_id', 'app_secret',
    'code', 'access_token',
    'redirect_uri',
]


def run():
    ConfigurationCenter.load_configuration()
    DatabaseHandler.open()
    while True:
        # for each keys, init handler and query.
        queries = []
        for info in ConfigurationCenter.generate_app_infos():
            args = [info[key] for key in ordered_keys_for_init]
            handler = WeiboAPIHandler(*args)
            query = PublicTimelineQuery(handler)
            queries.append(query)
        # timming.
        schedule = Schedule(DURATION)
        for query in queries:
            schedule.add_callback(query.query)
        schedule.run()
