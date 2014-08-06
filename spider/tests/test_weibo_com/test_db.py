
import unittest

from weibo_com.model import DB


class TestDB(unittest.TestCase):

    def setUp(self):
        pass

    def test_db(self):
        DB.add_user(uid='110',
                    name='jan',
                    followees=10,
                    fans=8,
                    num_post=1000)
        u = DB.get_user_by_uid('110')
        self.assertTrue(u.name == 'jan')


if __name__ == '__main__':
    unittest.main()
