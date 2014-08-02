from __future__ import (unicode_literals, print_function, absolute_import)

import collections


class Element(object):
    """
    @brief: Defines interface of element, an object to hold items such as url
            that would be process by ElementProcessor.
    """

    def set_processor(self, processor):
        self.processor = processor


class ElementProcessor(object):
    """
    @brief: Defines interface of element processor.
    """

    def process_element(self, element):
        """
        @brief: This funtion should be viewed as stateless by function caller.
        @return: A single Element object or an iterable that generates Element
                 objects.
        """
        raise NotImplementedError


class Strategy(object):
    """
    @brief: Defines interface of schedule strategy.
    """

    def valid(self):
        """
        @return: True if Strategy contains elements.
        """
        raise NotImplementedError

    def receive_element(self, element):
        raise NotImplementedError

    def next_element(self):
        """
        @return: element to be processed. None if nothing should be processed.
        """
        raise NotImplementedError


def simple_run_spider(strategy):
    while strategy.valid():
        next_element = strategy.next_element()
        processor = next_element.processor
        result = processor(next_element)

        if isinstance(result, collections.Iterable):
            for element in result:
                strategy.receive_element(element)
        elif result:
            strategy.receive_element(result)
