from __future__ import (unicode_literals, print_function, absolute_import)

import thread
import collections
from contextlib import contextmanager

from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy.orm.exc import NoResultFound
from sqlalchemy import ForeignKey
from sqlalchemy import func

from weibo_com.model import WeiboUser, Microblog


DB_URL = 'mysql://{}:{}@{}:{}/{}'.format(
    'root',
    '123456',
    'localhost',
    '3306',
    'sina',
)

Base = declarative_base()


class _WeiboUser(Base):
    __tablename__ = 'WeiboUser'

    uid = Column(String(255), primary_key=True)
    followees = Column(Integer)
    fans = Column(Integer)
    posts = Column(Integer)

    def __repr__(self):
        return "<User(uid='%s', name='%s', followees='%s', \
                fans='%s', num_post='%s')>" % (
            self.uid, self.name, self.followees,
            self.fans, self.num_post)


class _User2Blog(Base):
    __tablename__ = 'UserToBlog'

    mid = Column(String(255),
                 ForeignKey('Microblog.mid'),
                 primary_key=True)
    uid = Column(String(255), ForeignKey('WeiboUser.uid'))

    def __repr__(self):
        return "<User2Blog(mid='%s', uid='%s')>" % (
            self.mid,
            self.uid)


class _Microblog(Base):
    __tablename__ = 'Microblog'

    mid = Column(String(255), primary_key=True)
    created_time = Column(String(255))
    content = Column(String(255))
    favorites = Column(Integer)
    comments = Column(Integer)
    forwards = Column(Integer)
    forwarded_content = Column(String(255))

    def __repr__(self):
        return "<Microblog(mid='%s', time='%s', content='%s', \
                favorites='%s', comments='%s', forwards='%s', \
                forwarded_content='%s')>" % (
            self.mid,
            self.created_time,
            self.content,
            self.favorites,
            self.comments,
            self.forwards,
            self.forwarded_content)


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


def lock_and_unlock():
    lock = thread.allocate_lock()

    def _decorator(func):
        def _wrap(*args, **kwargs):
            lock.acquire()
            result = None
            try:
                result = func(*args, **kwargs)
            except Exception as e:
                print("Error in lock decorator: ", e)
            lock.release()
            return result
        return _wrap
    return _decorator


class DatabaseHandler:

    @classmethod
    def open(cls):
        cls.engine = create_engine(DB_URL)
        cls.Session = sessionmaker(bind=cls.engine)

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
        except:
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


class WeiboUserHandler(DatabaseHandler):

    EMPTY = -1

    # @lock_and_unlock()
    @classmethod
    @strong_filter('uid', followees=EMPTY, fans=EMPTY, posts=EMPTY)
    def add_user(cls, uid, **kwargs):
        with cls.modify_scope() as session:
            try:
                session.query(_WeiboUser).filter(_WeiboUser.uid == uid).one()
            except NoResultFound:
                user = _WeiboUser(uid=uid, **kwargs)
                session.add(user)

    # @lock_and_unlock()
    @classmethod
    @week_filter('uid', 'followees', 'fans', 'posts')
    def update_user(cls, uid, **kwargs):
        with cls.modify_scope() as session:
            user = session.query(_WeiboUser).filter_by(uid=uid).first()
            for key, value in kwargs.items():
                setattr(user, key, value)

    @classmethod
    @strong_filter('uid')
    def user_exist(cls, uid):
        with cls.query_scope() as session:
            try:
                session.query(_WeiboUser).filter(_WeiboUser.uid == uid).one()
            except NoResultFound:
                return False
            return True

    @classmethod
    @strong_filter('uid')
    def user_valid(cls, uid):
        with cls.query_scope() as session:
            user = session.query(_WeiboUser).filter_by(uid=uid).first()
            if user.followees != cls.EMPTY\
                    and user.fans != cls.EMPTY\
                    and user.posts != cls.EMPTY:
                return True
            return False

    @classmethod
    @strong_filter('uid')
    def get_user_by_uid(cls, uid):
        with cls.query_scope() as session:
            _u = session.query(_WeiboUser).filter_by(uid=uid).first()
            user = WeiboUser(_u.uid,
                             followees=_u.followees,
                             fans=_u.fans,
                             posts=_u.posts)
            return user

    # @lock_and_unlock()
    @classmethod
    @strong_filter('uid')
    def delete_user(cls, uid):
        with cls.modify_scope() as session:
            user = session.query(_WeiboUser).filter_by(uid=uid).first()
            session.delete(user)

    @classmethod
    def get_invalid_user(cls, num):
        with cls.query_scope() as session:
            count = session.query(func.count(_WeiboUser.uid)).\
                filter(_WeiboUser.posts == -1).first()
            if count < num:
                num = count
            uids = session.query(_WeiboUser.uid).\
                filter(_WeiboUser.posts == -1).\
                limit(num).all()
            return num, [item[0] for item in uids]


class MicroblogHandler(DatabaseHandler):

    @classmethod
    def add_blog(
            cls,
            uid,
            mid,
            created_time,
            content,
            favorites,
            comments,
            forwards,
            forwarded_content=None):
        with cls.modify_scope() as session:
            # try:
            #     session.query(_Microblog).filter(_Microblog.mid == mid)\
            #         .one()
            # except NoResultFound:
            b = _Microblog(
                mid=mid,
                created_time=created_time,
                content=content,
                favorites=favorites,
                comments=comments,
                forwards=forwards,
                forwarded_content=forwarded_content)
            session.add(b)
            u2b = _User2Blog(mid=mid, uid=uid)
            session.add(u2b)

    @classmethod
    def blog_exist(cls, mid):
        with cls.query_scope() as session:
            try:
                session.query(_Microblog).filter(_Microblog.mid == mid)\
                    .one()
            except NoResultFound:
                return False
            return True

    @classmethod
    def get_blog_by_mid(cls, mid):
        with cls.query_scope() as session:
            _b = session.query(_Microblog).filter_by(mid=mid).first()
            b = Microblog(
                _b.mid,
                _b.created_time,
                _b.content,
                _b.favorites,
                _b.comments,
                _b.forwards,
                _b.forwarded_content)
            return b

    @classmethod
    def delete_blog(cls, mid):
        with cls.modify_scope() as session:
            # delete entry of UserToBlog first
            u2b = session.query(_User2Blog).filter_by(mid=mid).first()
            session.delete(u2b)
            b = session.query(_Microblog).filter_by(mid=mid).first()
            session.delete(b)
