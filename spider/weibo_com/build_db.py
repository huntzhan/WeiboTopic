
from sqlalchemy import create_engine
from model import Base, DB_URL


def create_tables():
    engine = create_engine(DB_URL)
    Base.metadata.create_all(engine)


if __name__ == "__main__":
    create_tables()
