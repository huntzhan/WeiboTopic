
import time
from time import mktime


def ToTimestamp(_datetime):
    """
    @brief
    @input _datetime: format '09/10/2014 02:52'
    """
    _datetime = _datetime[0:-2] + '00'
    t_stru = time.strptime(_datetime, "%d/%m/%Y %H:%M")
    t_secs = long(mktime(t_stru))
    return str(t_secs)
