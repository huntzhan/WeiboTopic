
from setuptools import Command
from sqlalchemy import create_engine
from weibo_com.persist import Base, DB_URL


class BuildDBCommand(Command):

    user_options = [('testcmd', None, 'test')]

    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def create_tables(self):
        engine = create_engine(DB_URL)
        Base.metadata.create_all(engine)

    def run(self):
        self.create_tables()
