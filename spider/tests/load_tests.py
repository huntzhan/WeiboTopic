import os.path
import unittest


def additional_tests():
    loader = unittest.defaultTestLoader
    start_dir = os.path.join(
        os.getcwd(),
        'test_weibo_cn',
    )
    suites = loader.discover(start_dir)
    return suites
