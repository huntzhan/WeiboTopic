
from __future__ import (unicode_literals, print_function, absolute_import)

from contextlib import contextmanager

from sqlalchemy import Column, Integer, String
from sqlalchemy import create_engine
from sqlalchemy import ForeignKey
from sqlalchemy.orm import sessionmaker
from sqlalchemy.orm.exc import NoResultFound
from sqlalchemy.exc import IntegrityError
from sqlalchemy.ext.declarative import declarative_base

from .timestamp import NOW_IN_HOUR, anew_timestamp


DB_URL = 'mysql://{}:{}@{}:{}/{}?charset=utf8&use_unicode=0'.format(
    'root',
    '123456',
    'localhost',
    '3306',
    'sina',
)

Base = declarative_base()


def create_model_class(time_in_hour):
    """
    @brief: create new models and ensures that the corresponseding tables were
            created in database.
    """
    class _WeiboUser(Base):
        __tablename__ = 'WeiboUser{}'.format(time_in_hour)
        __table_args__ = {'mysql_engine': 'InnoDB'}

        uid = Column(String(50), primary_key=True)
        followees = Column(Integer)
        fans = Column(Integer)
        bi_followers_count = Column(Integer)
        posts = Column(Integer)
        sex = Column(String(1))
        favourites_count = Column(Integer)
        created_at = Column(String(50))
        verified = Column(Integer)

        def __repr__(self):
            return "<User%s(uid='%s')" % (time_in_hour, self.uid)

    class _Microblog(Base):
        __tablename__ = 'Microblog{}'.format(time_in_hour)
        __table_args__ = {'mysql_engine': 'InnoDB'}

        mid = Column(String(50), primary_key=True)
        created_time = Column(String(50))
        content = Column(String(255))
        favorites = Column(Integer)
        comments = Column(Integer)
        forwards = Column(Integer)
        source = Column(String(50))

        def __repr__(self):
            return "<Microblog%s(mid='%s')" % (time_in_hour, self.mid)

    class _User2Blog(Base):
        __tablename__ = 'UserToBlog{}'.format(time_in_hour)
        __table_args__ = {'mysql_engine': 'InnoDB'}

        mid = Column(String(50),
                     ForeignKey('Microblog{}.mid'.format(time_in_hour)),
                     primary_key=True)
        uid = Column(String(50),
                     ForeignKey('WeiboUser{}.uid'.format(time_in_hour)))

        def __repr__(self):
            return "<User2Blog%s(mid='%s', uid='%s')>" % (
                time_in_hour,
                self.mid,
                self.uid)

    # create tables here.
    # Base.metadata.create_all(cls.engine)
    return _WeiboUser, _Microblog, _User2Blog


class DatabaseHandler:

    @classmethod
    def switch_tables(cls):
        anew_timestamp()
        cls.User, cls.Blog, cls.UserToBlob = create_model_class(NOW_IN_HOUR)

    @classmethod
    def _table_exist(cls):
        cls.User, cls.Blog, cls.UserToBlob = create_model_class(NOW_IN_HOUR)
        if cls.User.__table__.exists():
            return True
        return False

    @classmethod
    def open(cls):
        cls.engine = create_engine(
            DB_URL,
            pool_size=0,
            pool_timeout=60,
        )
        cls.Session = sessionmaker(bind=cls.engine)
        if cls.table_exist(NOW_IN_HOUR):
            global Base
            Base.metadata.create_all(cls.engine)

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


class WeiboUserHandler(DatabaseHandler):

    EMPTY = -1

    @classmethod
    def add_user(cls, uid, **kwargs):
        with cls.modify_scope() as session:
            # try:
            #     session.query(_WeiboUser).filter(_WeiboUser.uid == uid).one()
            # except NoResultFound:
            #     user = _WeiboUser(uid=uid, **kwargs)
            #     session.add(user)
            user = cls.User(uid=uid, **kwargs)
            session.add(user)

    @classmethod
    def update_user(cls, uid, **kwargs):
        with cls.modify_scope() as session:
            user = session.query(cls.User).filter_by(uid=uid).first()
            for key, value in kwargs.items():
                setattr(user, key, value)

    @classmethod
    def user_exist(cls, uid):
        with cls.query_scope() as session:
            try:
                session.query(cls.User).filter(cls.User.uid == uid).one()
            except NoResultFound:
                return False
            return True

    @classmethod
    def delete_user(cls, uid):
        with cls.modify_scope() as session:
            user = session.query(cls.User).filter_by(uid=uid).first()
            session.delete(user)


class MicroblogHandler(DatabaseHandler):

    @classmethod
    def add_blog(cls, uid, mid, **kwargs):
        with cls.modify_scope() as session:
            # try:
            #     session.query(_Microblog).filter(_Microblog.mid == mid)\
            #         .one()
            # except NoResultFound:
            #     b = _Microblog(mid=mid, **kwargs)
            #     session.add(b)
            #     u2b = _User2Blog(mid=mid, uid=uid)
            #     session.add(u2b)
            b = cls.Blog(mid=mid, **kwargs)
            session.add(b)

        # make sure that foreign key exist.
        with cls.modify_scope() as session:
            u2b = cls.User2Blog(mid=mid, uid=uid)
            session.add(u2b)

    @classmethod
    def blog_exist(cls, mid):
        with cls.query_scope() as session:
            try:
                session.query(cls.Blog).filter(cls.Blog.mid == mid)\
                    .one()
            except NoResultFound:
                return False
            return True

    @classmethod
    def delete_blog(cls, mid):
        with cls.modify_scope() as session:
            # delete entry of UserToBlog first
            u2b = session.query(cls.User2Blog).filter_by(mid=mid).first()
            session.delete(u2b)
            b = session.query(cls.Blog).filter_by(mid=mid).first()
            session.delete(b)
