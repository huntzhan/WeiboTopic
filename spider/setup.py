#!/usr/bin/env python

from setuptools import setup, find_packages
import multiprocessing
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
        # 'Scrapy>=0.24.2',
        # dependencies of cola.
        'mechanize',
        'python-dateutil',
        'BeautifulSoup4',
        'mongoengine',
        'rsa',
        # dependencies of weibo_cn.
        'selenium',
        'requests>=2.3.0',
        'lxml',
        # dependencies of weibo_com.
        'sqlalchemy',
        'futures',
    ],

    packages=['easy_spider', 'weibo_com'],
    # entry_points={
    #     'console_scripts': [
    #         'somecmd = somepackage:main',
    #     ],
    # },
    test_suite='tests.load_tests',
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Programming Language :: Python :: 2.7',
        'Operating System :: OS Independent',
        'License :: OSI Approved :: MIT License',
    ],
)
