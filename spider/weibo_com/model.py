
from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine

from weibo_com.config import Config


Base = declarative_base()


class WeiboUser(Base):
    __tablename__ = 'WeiboUser'

    uid = Column(Integer, primary_key=True)
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
    engine = create_engine('mysql://%s:%s@%s:%s/%s' % (
                           conf['db_username'],
                           conf['db_password'],
                           conf['db_ip'],
                           conf['db_port'],
                           conf['db_name']
                           ))
    # create schemas
    WeiboUser.metadata.create_all(engine)

    Session = sessionmaker(bind=engine)
    session = Session()

    @classmethod
    def add_user(cls, uid, name=None, followees=0, fans=0, num_post=0):
        u = WeiboUser(uid=uid,
                      name=name,
                      followees=followees,
                      fans=fans,
                      num_post=num_post)
        cls.session.add(u)

    @classmethod
    def get_user_by_uid(cls, uid):
        u = cls.session.query(WeiboUser).filter_by(uid=uid).first()
        return u

    @classmethod
    def update_user(cls, uid, name=None, followees=-1, fans=-1, num_post=-1):
        u = cls.session.query(WeiboUser).filter_by(uid=uid).first()
        u.name = name if name is not None else 0
        u.followees = followees if followees != -1 else 0
        u.fans = fans if fans != -1 else 0
        u.num_post = num_post if num_post != 1 else 0
        cls.session.commit()

    @classmethod
    def del_user(cls, uid):
        u = cls.session.query(WeiboUser).filter_by(uid=uid).first()
        cls.session.delete(u)
        cls.session.flush()
