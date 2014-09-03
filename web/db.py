
import MySQLdb


class DBOperator:
    """
    get topics and sub_topics from db throught raw sql queries
    """

    def __init__(self, db_name='sina'):
        self.db_name = db_name
        self.username = 'root'
        self.passwd = '123456'
        self.conn = MySQLdb.connect(user=self.username,
                               passwd=self.passwd,
                               db=self.db_name,
                               charset='utf8',
                               use_unicode=True)

    def __enter__(self):
        return self

    def __exit__(self):
        self.conn.close()

    def GetOneHourTopics(self, timestamp, limit=0):
        """
        @brief read at most 'limit' rows(topics) from db, and
        return the top n based on the number of blogs in each topic
        @input timestamp: format 'xxx'
        @input limit: 0 for no limit
        @return None if table not exists
        """
        table_name = self._TimestampToTopicTablename(timestamp)
        cursor = self.conn.cursor()
        if self.IsTableExist(table_name):
            if limit == 0:
                cursor.execute("""select * from %s""" % (table_name))
                # cursor.execute("""select * from OneDayTopic_FilteredMicroblog1409670000""")
            else:
                cursor.execute("""select * from %s limit %s""", (table_name, limit))
            return cursor.fetchall()
        else:
            return None

    def GetSubTopics(self, timestamp, No_topic, limit):
        """
        @brief read the sub topic tables of one topic at the specific hour determined by timestamp
        @input limit: 0 for no limit
        """
        return [[], []];
        pass

    def IsTableExist(self, table_name):
        cursor = self.conn.cursor()
        cursor.execute("""SELECT count(*)
                       FROM information_schema.tables
                       WHERE table_schema = %s
                       AND table_name = %s""", (self.db_name, table_name))
        row = cursor.fetchone()
        if row[0] == 0L:
            return False
        else:
            return True

    def _TimestampToTopicTablename(self, timestamp):
        return "OneDayTopic_FilteredMicroblog" + timestamp

    def _TimestampToSubTopicTablename(self, timestamp, No_topic, No_sub):
        return "Sub_%s_Topic_FilteredMicroblog%s_%s" % (timestamp, No_sub, No_topic)
