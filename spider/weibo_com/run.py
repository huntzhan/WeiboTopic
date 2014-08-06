from __future__ import (unicode_literals, print_function, absolute_import)

from easy_spider import simple_run_spider
from .processor import FriendPageProcessor
from .element import UrlElement
from .search_strategy import BFS


def run():
    fans_page_processor = FriendPageProcessor()
    fans_page_processor.prepare_cookie_and_loader()

    fans_page = UrlElement("http://weibo.com/3211200050/follow?relate=fans")
    fans_page.set_processor(fans_page_processor)

    bfs = BFS()
    bfs.receive_element(fans_page)

    simple_run_spider(bfs)
