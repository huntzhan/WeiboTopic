# -*- coding: utf-8 -*-

import time
from time import mktime


def ToTimestamp(_datetime):
    """
    @brief
    @input _datetime: format '09/28/2014 02:52 PM'
    """
    am_pm = _datetime[-2:]
    hour = int(_datetime[-8:-6])
    if am_pm == 'PM':
        hour+= 12
    _datetime = _datetime[0:-8] + str(hour) + ':00'
    t_stru = time.strptime(_datetime, "%m/%d/%Y %H:%M")
    t_secs = long(mktime(t_stru))
    return str(t_secs)


def FromTimestamp(_timestamp):
    """
    @brief reverse the ToTimestamp
    """
    secs = float(_timestamp)
    t_stru = time.localtime(secs)
    t_str = time.asctime(t_stru)
    return t_str
