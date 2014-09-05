# -*- coding: utf-8 -*-

import sys
import getopt
import json

from flask import Flask
from flask import render_template
from flask import request

from model import Topic, SubTopic, CachedModel
from util import ToTimestamp

app = Flask(__name__)
app.config.update(dict(
    DEBUG=False
    # DEBUG=True
))


@app.route('/templates/subtopic_dialog.html')
def dialog():
    f = open('templates/subtopic_dialog.html')
    return f.read()


@app.route('/')
def index():
    tar_date = request.args.get('date', '')
    tar_time = request.args.get('time', '')
    if tar_time == '' or\
       tar_date == '' or\
       len(tar_time) != len("02:52 PM") or\
       len(tar_date) != len("09/10/2014"):
        return render_template('index.html')
    else:
        topics = []
        politic_topics = []
        timestamp = ToTimestamp(tar_date + ' ' + tar_time) # format: 09/28/2014 02:52 PM
        topics = CachedModel.GetOneHourTopic(timestamp)
        for topic in topics:
            if topic.is_politic == 1L:
                politic_topics.append(topic)
        if len(topics) != 0:
            return render_template('index.html', topics=topics, politic_topics=politic_topics)
        else:
            return render_template('index.html')
    # return redirect(url_for('show_entries'))
    # return render_template('index.html', topics=topics)


@app.route('/sub_topics')
def sub_topics():
    topic_id = request.args.get('id', 0, type=int)
    timestamp = request.args.get('timestamp', 0, type=str)
    print 'timestamp: ', timestamp, '   id: ', topic_id
    subtopics = CachedModel.GetSubTopics(timestamp, topic_id)
    subtopics_dict = {"sub_topics": []}
    subtopic_count = 0
    for subtopic in subtopics:
        subtopics_dict["sub_topics"].append(subtopic.jsonify())
    # print json.dumps(subtopics_dict, indent=4)
    return json.dumps(subtopics_dict)


@app.route('/about')
def about():
    return render_template('about.html')


@app.route('/metric')
def metrics():
    return render_template('metrics.html')


if __name__ == '__main__':
    # topic_db_name, topic_table_name = sys.argv[1].split('.')
    # subtopic_db_name, subtopic_table_name = sys.argv[2].split('.')

    # run the web server
    app.run(host='0.0.0.0')
    # app.run()
