
import unittest
from datetime import datetime
import pdb

from weibo_crawl.persist import WeiboUserHandler,\
    MicroblogHandler,\
    DatabaseHandler


class TestDB(unittest.TestCase):

    def setUp(self):
        handler = DatabaseHandler()
        handler.open()

    def tearDown(self):
        pass

    def test_user(self):
        self.handler = WeiboUserHandler()

        self.handler.add_user(
            uid='110',
            followees=10,
            fans=8,
            posts=1000,
            bi_followers_count=5,
            sex=1,
            favourites_count=101,
            created_at="craated_at",
            verified=0
        )
        # add duplicate user
        self.handler.add_user(
            uid='110',
            followees=10,
            fans=8,
            posts=1000,
            bi_followers_count=5,
            sex=1,
            favourites_count=101,
            created_at="craated_at",
            verified=0
        )

        self.handler.update_user(uid='110', followees=100)

        exist = self.handler.user_exist(uid='110')
        self.assertTrue(exist)
        self.handler.delete_user(uid='110')
        exist = self.handler.user_exist(uid='110')
        self.assertFalse(exist)

    def test_blog(self):
        # add blog with no user inserted before
        self.bhandler = MicroblogHandler()
        self.bhandler.add_blog(
            uid='110',
            mid='001',
            created_time=(str(datetime.today())),
            content='content',
            favorites=1,
            comments=1,
            forwards=1,
            source='source')
        self.assertFalse(self.bhandler.blog_exist('001'))

        # add blog with user inserted
        self.uhandler = WeiboUserHandler()
        self.uhandler.add_user(
            uid='110',
            followees=10,
            fans=8,
            posts=1000,
            bi_followers_count=5,
            sex=1,
            favourites_count=101,
            created_at="craated_at",
            verified=0
        )
        self.bhandler.add_blog(
            uid='110',
            mid='001',
            created_time=(str(datetime.today())),
            content='content',
            favorites=1,
            comments=1,
            forwards=1,
            source='source')
        self.assertTrue(self.bhandler.blog_exist('001'))

        # delete all
        self.bhandler.delete_blog(mid='001')
        self.uhandler.delete_user(uid='110')
        self.assertFalse(self.bhandler.blog_exist('001'))
        self.assertFalse(self.uhandler.user_exist(uid='110'))

if __name__ == '__main__':
    unittest.main()