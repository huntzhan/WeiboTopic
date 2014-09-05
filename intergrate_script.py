#!/usr/bin/env python
from __future__ import (unicode_literals, print_function, absolute_import)

import subprocess
import os
from tempfile import NamedTemporaryFile

from weibo_crawl.statistics import TableState
from weibo_crawl.bussiness import Schedule


DB_URL = 'mysql://{}:{}@{}:{}/{}?charset=utf8&use_unicode=0'.format(
    'root',
    '123456',
    '192.168.1.108',
    '3306',
    'sina',
)


ROOT = os.getcwd()
CLEAN_REL_PATH = 'weibo_ad_filter/build/run_filter'
TOPIC_REL_PATH = 'DataProcess/bin/run_dataprocess'
SUBTOPIC_REL_PATH = 'weibo_models/build/interface'

CLEAN_ABS_PATH = os.path.join(ROOT, CLEAN_REL_PATH)
TOPIC_ABS_PATH = os.path.join(ROOT, TOPIC_REL_PATH)
SUBTOPIC_ABS_PATH = os.path.join(ROOT, SUBTOPIC_REL_PATH)


group_adjacent = lambda a, k: zip(*([iter(a)] * k))


def make_list_item(items):
    for item in items:
        yield [item]


class CWDSwitcher(object):

    def __init__(self, program_path):
        self.current_cwd = os.getcwd()
        self.switch_cwd = os.path.dirname(program_path)

    def __enter__(self):
        os.chdir(self.switch_cwd)

    def __exit__(self, *args):
        os.chdir(self.current_cwd)


def call_procedure(command, input_sets):
    output_sets = []
    for args in input_sets:
        args = list(args)
        # temporary file.
        temp_file = NamedTemporaryFile()
        # set path of output file as first arguments.
        args.insert(0, temp_file.name)
        # insert command.
        args.insert(0, command)
        # run program.
        print(args)
        with CWDSwitcher(command):
            subprocess.call(args)
        # collect output.
        for line in filter(bool,
                           temp_file.read().split(os.linesep)):
            results = list(filter(bool, line.split(',')))
            output_sets.append(results)
        # discard temporary file.
        temp_file.close()
    return output_sets


class TopicGenerator(object):

    # recent 24 hours.
    # INTERVAL_LIMIT = 86400
    INTERVAL_LIMIT = 43200
    table_state = TableState(DB_URL, INTERVAL_LIMIT)

    @classmethod
    def _check_table_name(cls, table_name, table_name_without_epoch):
        _, target_name = cls.table_state.extract_key(table_name)
        assert target_name == table_name_without_epoch

    @classmethod
    def generator_recent_tables(cls):
        """
        @brief: a generator yield (Microblog, WeiboUser, UserToBlog).
        """
        static_tables = list(cls.table_state.get_static_tables())
        print("Size: ", len(static_tables) / 3)
        for message, user2message, user in\
                group_adjacent(static_tables, 3):
            cls._check_table_name(message, 'Microblog')
            cls._check_table_name(user, 'WeiboUser')
            cls._check_table_name(user2message, 'UserToBlog')

            add_prefix = lambda x: 'sina.' + x
            yield map(add_prefix, (message, user, user2message))


def classify_topic_outputs(tables):
    topic_tables = []
    message_tables = []
    for topic, messages in group_adjacent(tables, 2):
        topic_tables.extend(topic)
        message_tables.extend(messages)
    return topic_tables, message_tables


def intergration():
    # data cleaner.
    clean_tables = call_procedure(
        CLEAN_ABS_PATH,
        TopicGenerator.generator_recent_tables(),
    )

    # get topics.
    topic_tables = call_procedure(
        TOPIC_ABS_PATH,
        clean_tables,
    )
    topic_tables, message_tables = classify_topic_outputs(topic_tables)

    # process message_tables.
    sub_topic_tables = call_procedure(
        SUBTOPIC_ABS_PATH,
        make_list_item(message_tables),
    )


def main():
    ONE_HOUR = 3600.0
    intergration()
    while True:
        schedule = Schedule(ONE_HOUR)
        schedule.add_callback(intergration)
        schedule.run()


def test():
    output_sets = call_procedure(
        '/Users/haoxun/Temp/test_intergration/shit',
        [("InputA", "InputB", "IntputC")],
    )
    from pprint import pprint
    pprint(output_sets)


if __name__ == '__main__':
    main()
    # intergration()