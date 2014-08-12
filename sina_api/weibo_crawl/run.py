from __future__ import (unicode_literals, print_function, absolute_import)

from .bussiness import Schedule, PublicTimelineQuery


QUERY_PER_HOUT = 145
DURATION = 60.0 / QUERY_PER_HOUT


def run():
    while True:
        schedule = Schedule(DURATION)
        # for each keys, init handler and query.
        schedule.run()
