
import os

from weibo_com.error import DependencyNotInstalledError

try:
    import yaml
except ImportError:
    raise DependencyNotInstalledError('pyyaml')


class PropertyObject(dict):
    def __init__(self, d):
        super(PropertyObject, self).__init__()
        self._update(d)

    def _update(self, d):
        for k, v in d.iteritems():
            if not k.startswith('_'):
                self[k] = v

                if isinstance(v, dict):
                    setattr(self, k, PropertyObject(v))
                elif isinstance(v, list):
                    setattr(self, k, [PropertyObject(itm) for itm in v])
                else:
                    setattr(self, k, v)

    def update(self, config=None, **kwargs):
        self._update(kwargs)
        if config is not None:
            if isinstance(config, dict):
                self._update(config)
            else:
                self._update(config.conf)


class Config(object):
    def __init__(self, yaml_file):
        if isinstance(yaml_file, str):
            f = open(yaml_file)
        else:
            f = yaml_file
        try:
            self.conf = PropertyObject(yaml.load(f))
        finally:
            f.close()

        for k, v in self.conf.iteritems():
            if not k.startswith('_'):
                if isinstance(v, dict):
                    v = PropertyObject(v)
                setattr(self, k, v)

    def __getitem__(self, name):
        return getattr(self, name)


conf_dir = os.path.join(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__))),
    'weibo_com')
main_conf = Config(os.path.join(conf_dir, 'main.yaml'))

base = os.path.dirname(os.path.abspath(__file__))
user_conf = os.path.join(base, 'weibo.yaml')
user_config = Config(user_conf)

starts = [str(start.uid) for start in user_config.job.starts]

mongo_host = user_config.job.mongo.host
mongo_port = user_config.job.mongo.port
db_name = user_config.job.db

try:
    shard_key = user_config.job.mongo.shard_key
    shard_key = tuple([itm['key'] for itm in shard_key])
except AttributeError:
    shard_key = tuple()

instances = user_config.job.instances

fetch_forward = user_config.job.fetch.forward
fetch_comment = user_config.job.fetch.comment
fetch_like = user_config.job.fetch.like