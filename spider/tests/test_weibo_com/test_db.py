
import unittest

from weibo_com.model import WeiboUserHandler


class TestDB(unittest.TestCase):

    def setUp(self):
        self.handler = WeiboUserHandler()
        self.handler.open()

    def tearDown(self):
        self.handler.close()

    def test_everything(self):

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
        self.assertTrue(u is u1 and u1 is u2)

        self.assertTrue(u.uid == '110')
        self.assertFalse(u.uid == 110)
        self.assertTrue(type(u.uid) is str)
        self.assertFalse(type(u.uid) is int)
        self.assertFalse(type(u.uid) is long)
        self.assertTrue(type(u.followees) is long)

        self.handler.update_user(uid='110')
        self.handler.update_user(uid='110', followees=100)
        self.assertTrue(u.followees == 100)
        u = self.handler.get_user_by_uid(uid='110')
        self.assertTrue(u.followees == 100)

        exist = self.handler.user_exist(uid='110')
        self.assertTrue(exist)

        self.handler.delete_user(uid='110')
        exist = self.handler.user_exist(uid='110')
        self.assertFalse(exist)
