
from sqlalchemy.orm import sessionmaker
from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine


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
    engine = create_engine('sqlite:///:memory:', echo=False)
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
