"""
@brief: Defines db operations based on scope_session.
"""
from __future__ import (unicode_literals, print_function, absolute_import)

import logging

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker, scoped_session
from sqlalchemy.sql import exists

from .persist import (_WeiboUser, _User2Blog, _Microblog,
                      DB_URL)

logger = logging.getLogger(__name__)


engine = create_engine(
    DB_URL,
    pool_size=0,
    pool_timeout=60,
)

session_factory = sessionmaker(bind=engine)
Session = scoped_session(session_factory)


def strong_filter(*required_keys, **default_pairs):
    """
    @brief: decorator for function of class. kwargs MUST contains all keys of
            required_keys. default_pairs provides the default values. After
            decorating, class function could only accept keyword arguments.
    """

    def _decorator(func):
        """
        @input: keys could be a single object or an iterable. This function
                would extract corresponding values from kwargs, which should
                be a dictionary. If kwargs do not contains such keys, the
                function would just crash.
        @output: when keys is an iterable, return filter dictionary; when it
                 isn't, the function would just return the value pointed by
                 keys.
        """
        def _wrap(self, **kwargs):
            required_key_set = set(required_keys)
            default_key_set = set(default_pairs)
            input_key_set = set(kwargs)

            if not required_key_set <= input_key_set:
                raise RuntimeError("Missing requied keys.")

            processed_kwargs = {}
            for key in required_key_set:
                processed_kwargs[key] = kwargs[key] or\
                    default_pairs.get(key, None)

            for key in (default_key_set - required_key_set):
                processed_kwargs[key] = default_pairs[key]

            return func(self, **processed_kwargs)
        return _wrap
    return _decorator


def week_filter(*required_keys):
    def _decorator(func):
        def _wrap(self, **kwargs):
            processed_kwargs = {}
            for key, value in kwargs.items():
                if key in required_keys:
                    processed_kwargs[key] = value
            return func(self, **processed_kwargs)
        return _wrap
    return _decorator


class ThreadSafeHandler(object):

    def open(self):
        Session()

    def close(self):
        Session.remove()

    def commit(self):
        Session.commit()

    def _exist(self, condition):
        result = Session.query(exists().where(condition)).scalar()
        logger.info(result)
        return result
