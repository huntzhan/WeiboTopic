from __future__ import (unicode_literals, print_function, absolute_import)

from easy_spider import run_spider_asynchronously
from .processor import FriendPageProcessor
from .element import UrlElement
from .search_strategy import BFS
from .persist import DatabaseHandler


def _init_element(url):
    processor = FriendPageProcessor()
    processor.prepare_cookie_and_loader()
    url_ele = UrlElement(url)
    url_ele.set_processor(processor)
    return url_ele


def run():

    init_elements = [
        _init_element("http://weibo.com/3211200050/follow?relate=fans"),
        _init_element("http://weibo.com/1898353550/follow?relate=fans"),
        _init_element("http://weibo.com/1671222043/follow?relate=fans"),
        _init_element("http://weibo.com/1737823322/follow?relate=fans"),
    ]

    db = DatabaseHandler()
    db.open()

    bfs = BFS()
    for ele in init_elements:
        bfs.receive_element(ele, force=True)

    run_spider_asynchronously(bfs, 4)
