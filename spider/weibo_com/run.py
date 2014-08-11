from __future__ import (unicode_literals, print_function, absolute_import)

from easy_spider import run_spider_asynchronously
from .processor import FriendPageProcessor
from .element import UrlElement
from .search_strategy import RandomInvalidUserAccessor
from .persist import DatabaseHandler


def _init_element(url):
    processor = FriendPageProcessor()
    url_ele = UrlElement(url, processor)
    return url_ele


def crawl_user_info():

    init_elements = [
        _init_element("http://weibo.com/3211200050/follow?relate=fans"),
        _init_element("http://weibo.com/1898353550/follow?relate=fans"),
        # _init_element("http://weibo.com/1671222043/follow?relate=fans"),
        # _init_element("http://weibo.com/1737823322/follow?relate=fans"),
    ]

    db = DatabaseHandler()
    db.open()

    strategy = RandomInvalidUserAccessor()
    for ele in init_elements:
        strategy.receive_element(ele)

    run_spider_asynchronously(strategy, 2)
