
import unittest
from db import DBOperator


class TestDB(unittest.TestCase):
    def setUp(self):
        self.demo = '1409670000'
        self.db_handler = DBOperator()

    def test_table_exist(self):
        self.table_name = self.db_handler._TimestampToTopicTablename(self.demo)
        self.assertTrue('OneDayTopic_FilteredMicroblog1409670000' == self.table_name)
        is_exist = self.db_handler.IsTableExist(self.table_name)
        self.assertTrue(is_exist)

    def test_get_topic(self):
        rows = self.db_handler.GetOneHourTopics(self.demo)
        for r in rows:
            print r[1].encode('utf8')

    def test_get_sub_topic(self):
        No_topic = 1
        sub_topics = self.db_handler.GetSubTopics(self.demo, No_topic)
        for sub_topic in sub_topics:
            for row in sub_topic:
                print row[0]
                print row[1]
                print row[2]


if __name__ == '__main__':
    unittest.main()
