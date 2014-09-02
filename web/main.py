from flask import Flask, render_template

from model import Topic, SubTopic

app = Flask(__name__)
app.config.update(dict(
    # DEBUG=False
    DEBUG=True
))

keywords = []
for _ in range(1, 6):
    keywords.append("keyword" + str(_))
blogs = []
for _ in range(1, 6):
    blogs.append("blog" + str(_))
sub_topics = []
for _ in range(1, 6):
    sub = SubTopic(keywords, 5, blogs)
    sub_topics.append(sub)


@app.route('/')
def index():
    topics = []
    for i in range(1, 11):
        topic = Topic("Topic"+str(i), keywords, 5, sub_topics)
        topics.append(topic)
    return render_template('index.html', topics=topics)


@app.route('/time')
def time():
    return render_template('time.html')


@app.route('/about')
def about():
    return render_template('about.html')


@app.route('/metric')
def metrics():
    return render_template('metrics.html')


if __name__ == '__main__':
    app.run(host='0.0.0.0')
    # app.run()
