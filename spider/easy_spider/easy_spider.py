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
    @brief: Defines common interface of schedule strategy.
    """

    def valid(self):
        """
        @return: True if Strategy contains elements.
        """
        raise NotImplementedError

    def receive_element(self, element):
        raise NotImplementedError


class SequentialStrategy(Strategy):

    def next_element(self):
        """
        @return: element to be processed. None if nothing should be processed.
        """
        raise NotImplementedError


class ConcurrentStrategy(Strategy):
    """
    @brief: Defines additional interfaces for concurrency.
    """

    def next_independent_elements(self, max):
        """
        @return: Itarable that generates elements that are bounded to
                 independent processor, length of returned iterable should
                 be <= max.
        """
        raise NotImplementedError


def simple_run_spider(strategy):
    while strategy.valid():
        next_element = strategy.next_element()
        processor = next_element.processor
        result = processor.process_element(next_element)

        if isinstance(result, collections.Iterable):
            for element in result:
                strategy.receive_element(element)
        elif result:
            strategy.receive_element(result)


def run_spider_asynchronously(strategy, max_worker=2, error_recovery=True):
    # For some unknown reason, importing concurrent.futures on top-level
    # module would cause './setup.py test' crash.
    from concurrent.futures import ThreadPoolExecutor

    while strategy.valid():
        with ThreadPoolExecutor(max_worker) as executor:
            # retrive elements.
            elements = [ele for ele in
                        strategy.next_independent_elements(max_worker)]
            if len(elements) > max_worker:
                raise Exception("Exceeded maximum worker.")
            elif len(elements) == 0:
                raise Exception("No element!")

            # submits.
            futures = []
            for element in elements:
                func = element.processor.process_element
                future = executor.submit(func, element)
                futures.append(future)
            # get results.
            for future in futures:
                print("Fetch result.")
                try:
                    result = future.result()
                except Exception as e:
                    print("ERROR in run_spider_asynchronously: ", e)
                    continue

                if isinstance(result, collections.Iterable):
                    for element in result:
                        strategy.receive_element(element)
                elif result:
                    strategy.receive_element(result)
