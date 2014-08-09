
import unittest
from datetime import datetime

from weibo_com.model import WeiboUser, Microblog
from weibo_com.persist import WeiboUserHandler,\
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
        )
        # add duplicate user
        self.handler.add_user(
            uid='110',
            followees=10,
            fans=8,
            posts=1000,
        )

        u = self.handler.get_user_by_uid(uid='110')
        u1 = self.handler.get_user_by_uid(uid='110')
        u2 = self.handler.get_user_by_uid(uid='110')
        self.assertFalse(u is u1 or u1 is u2)

        self.assertTrue(u.uid == '110')
        self.assertFalse(u.uid == 110)
        self.assertTrue(type(u.uid) is str)
        self.assertFalse(type(u.uid) is int)
        self.assertFalse(type(u.uid) is long)
        self.assertTrue(type(u.followees) is long)

        self.handler.update_user(uid='110')
        self.handler.update_user(uid='110', followees=100)
        self.assertFalse(u.followees == 100)
        u4 = self.handler.get_user_by_uid(uid='110')
        self.assertTrue(u4.followees == 100)

        exist = self.handler.user_exist(uid='110')
        self.assertTrue(exist)
        self.handler.delete_user(uid='110')
        exist = self.handler.user_exist(uid='110')
        self.assertFalse(exist)

    def test_blog(self):
        self.uhandler = WeiboUserHandler()

        self.uhandler.add_user(
            uid='110',
            followees=10,
            fans=8,
            posts=1000,
        )
        # add duplicate user
        self.bhandler = MicroblogHandler()

        self.bhandler.add_blog(
            uid='110',
            mid='001',
            created_time=(str(datetime.today())),
            content='content',
            favorites=1,
            comments=1,
            forwards=1,
            forwarded_content=None)

        self.bhandler.delete_blog(mid='001')
        self.uhandler.delete_user(uid='110')
        self.assertFalse(self.bhandler.blog_exist('001'))
        self.assertFalse(self.uhandler.user_exist(uid='110'))
