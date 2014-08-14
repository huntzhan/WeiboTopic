
from __future__ import (unicode_literals, print_function, absolute_import)

import time

from sqlalchemy import Column, Integer, String
from sqlalchemy import ForeignKey
from sqlalchemy.ext.declarative import declarative_base

from .db import engine


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

    return _WeiboUser, _Microblog, _User2Blog


class ModelManager(object):
    """
    @brief: 1. manage a mapping from time.struct_time to models.
            2. responsible for creating new module.
    """

    time_models_mapping = {}

    @classmethod
    def _round_to_hour(cls, timestamp):
        """
        @input: timestamp is a time.struct_time object.
        @output: a time.struct_time rounded from timestamp.
        """

        time_format = '%Y %m %d %H'
        rounded_time_string = timestamp.strftime(time_format)
        return time.strptime(rounded_time_string, time_format)

    @classmethod
    def _create_new_models(cls, rounded_timestamp):
        """
        @brief: convert time.struct_time to epoch, create models, keep mapping,
                then return newly created models.
        """
        suffix = str(
            int(time.mktime(rounded_timestamp)))
        models = create_model_class(suffix)
        for m in models:
            t = m.__table__
            # test table existence
            if not t.exists(bind=engine):
                # create tables here.
                t.create(engine, checkfirst=True)
        cls.time_models_mapping[rounded_timestamp] = models
        return models

    @classmethod
    def get_models(cls, timestamp):
        rounded_timestamp = cls._round_to_hour()
        models = cls.time_models_mapping.get(rounded_timestamp, None)
        if models:
            return models
        else:
            return cls._create_new_models(rounded_timestamp)
