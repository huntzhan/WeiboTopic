
import unittest

from weibo_com.model import DB


class TestDB(unittest.TestCase):

    def setUp(self):
        pass

    def test_db(self):
        if not DB.is_session_open():
            DB.open()

        DB.add_user(uid='110',
                    name='jan',
                    followees=10,
                    fans=8,
                    num_post=1000)
        # add duplicate user
        DB.add_user(uid='110',
                    name='jan',
                    followees=10,
                    fans=8,
                    num_post=1000)
        u = DB.get_user_by_uid('110')
        _u = DB.get_user_by_uid('110')
        __u = DB.get_user_by_uid('110')
        self.assertTrue(u is _u and _u is __u)
        self.assertTrue(u.name == 'jan')
        self.assertTrue(u.uid == '110')
        self.assertFalse(u.uid == 110)
        self.assertTrue(type(u.uid) is str)
        self.assertFalse(type(u.uid) is int)
        self.assertFalse(type(u.uid) is long)
        self.assertTrue(type(u.followees) is long)

        DB.update_user('110', followees=100)
        self.assertTrue(u.followees == 100)
        u = DB.get_user_by_uid('110')
        self.assertTrue(u.followees == 100)

        is_exist = DB.is_user_exist('110')
        self.assertTrue(is_exist)

        DB.del_user('110')
        is_exist = DB.is_user_exist('110')
        self.assertFalse(is_exist)

    def tearDown(self):
        DB.close()
