
import unittest
import json
import pdb
from model import CachedModel


class TestModel(unittest.TestCase):
    def test_cachemodel(self):
        demo = '1409670000'
        topics = CachedModel.GetOneHourTopic(demo)
        for t in topics:
            print t
            print ''

    def test_cachemodel(self):
        demo = '1409670000'
        No_topic = 1
        subtopics = CachedModel.GetSubTopics(demo, No_topic)
        for sub in subtopics:
            print sub

    def test_sub_topic_json(self):
        demo = '1409670000'
        No_topic = 1
        subtopics = CachedModel.GetSubTopics(demo, No_topic)
        subtopics_dict = {}
        subtopic_count = 0
        for subtopic in subtopics:
            subtopics_dict[str(++subtopic_count)] = subtopic.jsonify()
        print json.dumps(subtopics_dict, indent=4)


if __name__ == '__main__':
    unittest.main()
