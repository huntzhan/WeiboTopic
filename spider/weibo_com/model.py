
import collections

from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy.orm.exc import NoResultFound


DB_URL = 'mysql://{}:{}@{}:{}/{}'.format(
    'root',
    '123456',
    'localhost',
    '3306',
    'sina',
)

Base = declarative_base()


class WeiboUser(Base):
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

    def open(self):
        engine = create_engine(DB_URL)
        Session = sessionmaker(bind=engine)
        # here's what we want.
        self.session = Session()

    def close(self):
        self.session.close_all()


class WeiboUserHandler(DatabaseHandler):

    EMPTY = 'empty'

    @strong_filter('uid', followees=0, fans=0, posts=0)
    def add_user(self, uid, **kwargs):

        if self.user_exist(uid=uid):
            return

        user = WeiboUser(uid=uid, **kwargs)
        self.session.add(user)
        self.session.commit()

    @week_filter('uid', 'followees', 'fans', 'posts')
    def update_user(self, uid, **kwargs):
        user = self.session.query(WeiboUser).filter_by(uid=uid).first()
        for key, value in kwargs.items():
            setattr(user, key, value)
        self.session.commit()

    @strong_filter('uid')
    def user_exist(self, uid):
        try:
            self.session.query(WeiboUser).filter(WeiboUser.uid == uid).one()
        except NoResultFound:
            return False
        return True

    @strong_filter('uid')
    def get_user_by_uid(self, uid):
        user = self.session.query(WeiboUser).filter_by(uid=uid).first()
        return user

    @strong_filter('uid')
    def delete_user(self, uid):
        user = self.session.query(WeiboUser).filter_by(uid=uid).first()
        self.session.delete(user)
        self.session.commit()
