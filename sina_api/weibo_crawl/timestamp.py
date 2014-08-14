# import time
#
#
# def _round_hour(time_in_sec):
#     """
#     cut the arg into last o'clock
#     @return o'clock time in sec
#     """
#     return time_in_sec - (time_in_sec % 3600)
#
# NOW_IN_HOUR = _round_hour(time.mktime(time.gmtime()))
#
#
# def anew_timestamp():
#     global NOW_IN_HOUR
#     NOW_IN_HOUR = _round_hour(time.mktime(time.gmtime()))
