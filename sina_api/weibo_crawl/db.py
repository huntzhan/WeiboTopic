
from sqlalchemy import create_engine

DB_URL = 'mysql://{}:{}@{}:{}/{}?charset=utf8&use_unicode=0'.format(
    'root',
    '123456',
    'localhost',
    '3306',
    'sina',
)

engine = None


def open_engine():
    global engine
    engine = create_engine(
        DB_URL,
        pool_size=0,
        pool_timeout=60,
    )
