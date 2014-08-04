from __future__ import (unicode_literals, print_function, absolute_import)

from easy_spider import simple_run_spider
from .processor import FansPageProcessor
from .element import UrlElement
from .search_strategy import BFS


def run():
    fans_page = UrlElement("http://weibo.cn/1191258123/fans")
    fans_page_processor = FansPageProcessor()
    fans_page.set_processor(fans_page_processor)

    bfs = BFS()
    bfs.receive_element(fans_page)

    simple_run_spider(bfs)
