from __future__ import (unicode_literals, print_function, absolute_import)

from threading import Timer
from pprint import pprint
import time


def fucking_print(count):
    # while True:
    #     print(count)
    print(count)


def mission(count, offset):
    # sleep with offset.
    t = Timer(offset, fucking_print, [count])
    t.start()


def run_mission():
    for _ in range(10):
        mission(1.0, 1)
        mission(2.0, 2)
        time.sleep(2)


if __name__ == "__main__":
    run_mission()
