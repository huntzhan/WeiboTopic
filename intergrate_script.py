#!/usr/bin/env python
from __future__ import (unicode_literals, print_function, absolute_import)

import subprocess
import os
from tempfile import NamedTemporaryFile

# from weibo_crawl.statistics import TableState


ROOT = os.getcwd()
CLEAN_REL_PATH = 'path/to/clean'
TOPIC_REL_PATH = 'path/to/topic'
SUBTOPIC_REL_PATH = 'path/to/subtopic'

CLEAN_ABS_PATH = os.path.join(ROOT, CLEAN_REL_PATH)
TOPIC_ABS_PATH = os.path.join(ROOT, TOPIC_REL_PATH)
SUBTOPIC_ABS_PATH = os.path.join(ROOT, SUBTOPIC_REL_PATH)


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
        subprocess.call(args)
        # collect output.
        for line in filter(bool,
                           temp_file.read().split(os.linesep)):
            output_sets.append(line.split(','))
        # discard temporary file.
        temp_file.close()
    return output_sets


def main():
    pass


def test():
    output_sets = call_procedure(
        '/Users/haoxun/Temp/test_intergration/shit',
        [("InputA", "InputB", "IntputC")],
    )
    from pprint import pprint
    pprint(output_sets)


if __name__ == '__main__':
    test()
