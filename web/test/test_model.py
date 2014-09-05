
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

    def test_sub_topic_json(self):
        demo = '1408179600'
        metrics_path = '/tmp/JavaProjct-jinfa/OutPut'
        matched_metrics, match_mainideas, unmatched_metrics = \
            CachedModel.GetMetrics(metrics_path, demo)
        for item in matched_metrics:
            print item, match_mainideas[item]
        print ''
        for item in unmatched_metrics:
            print item
        self.assertTrue(len(matched_metrics) > 0)
        self.assertTrue(len(match_mainideas) > 0)
        self.assertTrue(len(unmatched_metrics) > 0)


if __name__ == '__main__':
    unittest.main()
