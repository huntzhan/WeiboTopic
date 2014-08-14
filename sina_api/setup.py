#!/usr/bin/env python

from setuptools import setup, find_packages
# from some_interface import __version__

setup(
    name='',
    version='0.1',
    author='',
    author_email='',
    url='',
    license='MIT',
    description='',
    # long_description=open('README.rst').read(),
    install_requires=[
        'requests>=2.3.0',
        'sqlalchemy',
        'futures',
        'mysql-python',
        'selenium',
        'configparser',
    ],
    packages=[],
    entry_points={
        "distutils.commands": [
            "build_db = weibo_crawl.build_db:BuildDBCommand",
        ],
        'console_scripts': [
            'run_api = weibo_crawl.run:api',
            'run_report = weibo_crawl.run:report',
        ],
    },
    test_suite='tests.load_tests',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python :: 2.7',
        'Operating System :: OS Independent',
        'License :: OSI Approved :: MIT License',
    ],
)
