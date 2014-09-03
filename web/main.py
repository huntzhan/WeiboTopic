import sys
import getopt

from flask import Flask
from flask import render_template
from flask import request

from model import Topic, SubTopic, CachedModel
from util import ToTimestamp

app = Flask(__name__)
app.config.update(dict(
    # DEBUG=False
    DEBUG=True
))


@app.route('/')
def index():
    tar_time = request.args.get('time', '')
    if tar_time == '':
        return render_template('index.html')
    else:
        topics = []
        # 09/10/2014 02:52
        # get topics
        timestamp = ToTimestamp(tar_time)
        topics = CachedModel.GetOneHourTopic(timestamp)
        if len(topics) == 0:
            return render_template('index.html', topics=topics)
        else:
            return render_template('index.html')
    # return redirect(url_for('show_entries'))
    # return render_template('index.html', topics=topics)


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
