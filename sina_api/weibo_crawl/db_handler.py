"""
@brief: Defines db operations based on scope_session.
"""
from __future__ import (unicode_literals, print_function, absolute_import)

import logging

from sqlalchemy.orm import sessionmaker
from sqlalchemy.sql import exists

from .model_manager import ModelManager
from .persist import engine


logger = logging.getLogger(__name__)


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


# from sqlalchemy.orm import scoped_session
#
#
# session_factory = sessionmaker(bind=engine)
# Session = scoped_session(session_factory)
#
#
# class ThreadSafeHandler(object):
#
#     @classmethod
#     def open(self):
#         Session()
#
#     @classmethod
#     def close(self):
#         Session.remove()
#
#     @classmethod
#     def commit(self):
#         Session.commit()
#
#     @classmethod
#     def _exist(self, condition):
#         result = Session.query(exists().where(condition)).scalar()
#         logger.info(result)
#         return result


class DatabaseHandler:

    @classmethod
    def open(cls):
        cls.Session = sessionmaker(bind=engine)

    @classmethod
    def close(cls):
        pass

    @classmethod
    @contextmanager
    def modify_scope(cls):
        """Provide a transactional scope around a series of operations."""
        session = cls.Session()
        try:
            yield session
            session.commit()
        except IntegrityError:
            session.rollback()
        finally:
            session.close()

    @classmethod
    @contextmanager
    def query_scope(cls):
        """Provide a transactional scope around a series of operations."""
        session = cls.Session()
        try:
            yield session
        finally:
            session.close()

    @classmethod
    def add_entry(cls, user, msg):
        u_dict = dict(user._asdict())
        b_dict = dict(msg._asdict())
        # examine timestamp of msg
        t_str = b_dict['created_time']
        t_struct = time.strptime(t_str, "%a %b %d %H:%M:%S +0800 %Y")
        models = ModelManager.get_models(t_struct)
        # add user
        with cls.modify_scope() as session:
            # try:
            #     session.query(_WeiboUser).filter(_WeiboUser.uid == uid).one()
            # except NoResultFound:
            #     user = _WeiboUser(uid=uid, **kwargs)
            #     session.add(user)
            u = models[0](**u_dict)
            session.add(u)
        # add blog
        with cls.modify_scope() as session:
            # try:
            #     session.query(_Microblog).filter(_Microblog.mid == mid)\
            #         .one()
            # except NoResultFound:
            #     b = _Microblog(mid=mid, **kwargs)
            #     session.add(b)
            #     u2b = _User2Blog(mid=mid, uid=uid)
            #     session.add(u2b)

            b = models[1](**b_dict)
            session.add(b)
        # add User2Blog
        # make sure that foreign key exist.
        uid = u_dict['uid']
        mid = b_dict['mid']
        with cls.modify_scope() as session:
            u2b = models[2](mid=mid, uid=uid)
            session.add(u2b)
