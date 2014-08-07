
from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy.orm.exc import NoResultFound

from weibo_com.config import Config


Base = declarative_base()


class WeiboUser(Base):
    __tablename__ = 'WeiboUser'

    uid = Column(String(255), primary_key=True)
    name = Column(String(255))
    followees = Column(Integer)
    fans = Column(Integer)
    num_post = Column(Integer)

    def __repr__(self):
        return "<User(uid='%s', name='%s', followees='%s', \
                fans='%s', num_post='%s')>" % (
            self.uid, self.name, self.followees,
            self.fans, self.num_post)


class DB:
    # connect to db
    # URL syntax: dialect+driver://username:password@host:port/database
    # engine = create_engine('sqlite:///:memory:', echo=False)
    conf = Config.values
    engine = None
    try:
        dburl = 'mysql://{}:{}@{}:{}/{}'.format(
            conf['db_username'],
            conf['db_password'],
            conf['db_ip'],
            conf['db_port'],
            conf['db_name']
            )
        engine = create_engine(dburl)
    except:
        raise Exception("fail to create engine for MySQL \
                        with URL: {}".format(
                        dburl))
    # create schemas
    try:
        WeiboUser.metadata.create_all(engine)
    except:
        raise Exception("fail to create table schema in db")

    session = None

    @classmethod
    def open(cls):
        try:
            Session = sessionmaker(bind=cls.engine)
            cls.session = Session()
        except:
            raise Exception("fail to create db session \
                            with engine {}".format(
                            cls.engine))

    @classmethod
    def is_session_open(cls):
        if cls.session is None:
            return False
        return True

    @staticmethod
    def tostr(arg):
        if arg is None:
            return None
        if not isinstance(arg, str):
            try:
                return str(arg)
            except:
                raise Exception("'tostr()' fails: {}".format(arg))
        return arg

    @staticmethod
    def toint(arg):
        if arg is None:
            return None
        if not isinstance(arg, int):
            try:
                return int(arg)
            except:
                raise Exception("'toint()' fails: {}".format(arg))
        return arg

    @classmethod
    def add_user(cls, uid, name=None, followees=0, fans=0, num_post=0):
        if cls.is_user_exist(uid):
            return

        uid = cls.tostr(uid)
        name = cls.tostr(name)
        followees = cls.toint(followees)
        fans = cls.toint(fans)
        num_post = cls.toint(num_post)
        u = WeiboUser(uid=uid,
                      name=name,
                      followees=followees,
                      fans=fans,
                      num_post=num_post)
        cls.session.add(u)
        cls.session.commit()

    @classmethod
    def is_user_exist(cls, uid):
        uid = cls.tostr(uid)
        try:
            cls.session.query(WeiboUser).filter(WeiboUser.uid == uid).one()
        except NoResultFound:
            return False
        return True

    @classmethod
    def get_user_by_uid(cls, uid):
        uid = cls.tostr(uid)
        u = cls.session.query(WeiboUser).filter_by(uid=uid).first()
        return u

    @classmethod
    def update_user(cls, uid, name=None, followees=-1, fans=-1, num_post=-1):
        uid = cls.tostr(uid)
        name = cls.tostr(name)
        followees = cls.toint(followees)
        fans = cls.toint(fans)
        num_post = cls.toint(num_post)

        u = cls.session.query(WeiboUser).filter_by(uid=uid).first()
        u.name = u.name if name is None else name
        u.followees = u.followees if followees == -1 else followees
        u.fans = u.fans if fans == -1 else fans
        u.num_post = u.num_post if num_post == -1 else num_post
        cls.session.commit()

    @classmethod
    def del_user(cls, uid):
        uid = cls.tostr(uid)
        u = cls.session.query(WeiboUser).filter_by(uid=uid).first()
        cls.session.delete(u)
        cls.session.commit()

    @classmethod
    def close(cls):
        cls.session.close_all()
        cls.session = None
