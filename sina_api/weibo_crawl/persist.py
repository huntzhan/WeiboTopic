
from __future__ import (unicode_literals, print_function, absolute_import)

import time

from sqlalchemy import create_engine
from sqlalchemy import Column, Integer, String
from sqlalchemy import ForeignKey
from sqlalchemy.ext.declarative import declarative_base


DB_URL = 'mysql://{}:{}@{}:{}/{}?charset=utf8&use_unicode=0'.format(
    'root',
    '123456',
    'localhost',
    '3306',
    'sina',
)

engine = create_engine(
    DB_URL,
    pool_size=0,
    pool_timeout=60,
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
    for model in [_WeiboUser, _Microblog, _User2Blog]:
        table = model.__table__
        # test table existence
        if not table.exists(bind=engine):
            # create tables here.
            table.create(engine, checkfirst=True)
    return _WeiboUser, _Microblog, _User2Blog
