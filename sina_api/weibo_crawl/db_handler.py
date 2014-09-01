"""
@brief: Defines db operations based on scope_session.
"""
from __future__ import (unicode_literals, print_function, absolute_import)

import thread
import time
from contextlib import contextmanager
import logging
from collections import defaultdict

from sqlalchemy.orm import sessionmaker
from sqlalchemy.sql import exists
from sqlalchemy.exc import IntegrityError, TimeoutError
from sqlalchemy.orm import scoped_session

from .model_manager import ModelManager
from .persist import engine


session_factory = sessionmaker(bind=engine)
Session = scoped_session(session_factory)

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
        try:
            session = cls.Session()
            yield session
            session.commit()
        except (IntegrityError, TimeoutError) as e:
            logger.warning(e)
            session.rollback()
        finally:
            session.close()

    @classmethod
    @contextmanager
    def query_scope(cls):
        """Provide a transactional scope around a series of operations."""
        try:
            session = cls.Session()
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


# class DatabaseConnection(object):
#     Session = sessionmaker(bind=engine)
#
#     @classmethod
#     def open(cls):
#         # open connection.
#         cls.session = cls.Session()
#
#     @classmethod
#     def close(cls):
#         try:
#             cls.session.commit()
#         except (IntegrityError, TimeoutError) as e:
#             logger.warning(e)
#             session.rollback()
#         finally:
#             session.close()
#
#     @classmethod
#     def add_users_and_messages(cls, users, messages):
#         user_instances = []
#         message_instances = []
#         user_message_relations = []
#
#         for user, message in zip(users, messages):
#             user_dict = dict(user._asdict())
#             message_dict = dict(message._asdict())
#             uid = user_dict['uid']
#             mid = message_dict['mid']
#
#             # get models.
#             raw_time = message_dict['created_time']
#             timestamp = time.strptime(raw_time, "%a %b %d %H:%M:%S +0800 %Y")
#             models = ModelManager.get_models(timestamp)
#
#             # create instance for merge.
#             user_instances.append(models[0](**user_dict))
#             message_instances.append(models[1](**message_dict))
#             user_message_relations.append(models[2](uid=uid, mid=mid))
#
#         for user_instance, message_instance in\
#                 zip(user_instances, message_instances):
#             cls.session.merge(user_instance)
#             cls.session.merge(message_instance)
#         for u2b_instance in user_message_relations:
#             cls.session.merge(u2b_instance)


def lock_and_unlock():
    lock = thread.allocate_lock()

    def _decorator(func):
        def _wrap(*args, **kwargs):
            lock.acquire()
            result = None
            try:
                result = func(*args, **kwargs)
            except Exception as e:
                logger.warning("Error in lock decorator: ", e)
            lock.release()
            return result
        return _wrap
    return _decorator


class ThreadSafeHandler(object):

    @classmethod
    def open(self):
        Session()

    @classmethod
    def close(self):
        Session.remove()

    @classmethod
    def commit(self):
        try:
            Session.commit()
        except (IntegrityError, TimeoutError) as e:
            logger.warning(e)
            Session.rollback()

    @classmethod
    def _raw_sql_insert_or_update(cls, model, instances):
        table_object = model.__table__
        inserter = table_object.insert().prefix_with("IGNORE")
        Session.execute(inserter, instances)

    @classmethod
    @lock_and_unlock()
    def add_users_and_messages(cls, users, messages):
        model_user_mapping = defaultdict(list)
        model_message_mapping = defaultdict(list)
        model_u2m_mapping = defaultdict(list)

        for user, message in zip(users, messages):
            user_dict = dict(user._asdict())
            message_dict = dict(message._asdict())
            uid = user_dict['uid']
            mid = message_dict['mid']

            # get models.
            raw_time = message_dict['created_time']
            timestamp = time.strptime(raw_time, "%a %b %d %H:%M:%S +0800 %Y")
            models = ModelManager.get_models(timestamp)

            # create instance for merge.
            model_user_mapping[models[0]].append(user_dict)
            model_message_mapping[models[1]].append(message_dict)
            model_u2m_mapping[models[2]].append({'uid': uid, 'mid': mid})

        for mapping in [model_user_mapping,
                        model_message_mapping,
                        model_u2m_mapping]:
            for model, instances in mapping.items():
                cls._raw_sql_insert_or_update(model, instances)
