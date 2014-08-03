from __future__ import (unicode_literals, print_function, absolute_import)

from easy_spider import Element


class UrlElement(Element):

    def __init__(self, url):
        self.url = url
