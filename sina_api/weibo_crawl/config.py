from __future__ import (unicode_literals, print_function, absolute_import)

import configparser


def week_filter(*required_keys):
    def _decorator(func):
        def _wrap(self, **kwargs):
            processed_kwargs = {}
            for key, value in kwargs.items():
                if key in required_keys:
                    processed_kwargs[key] = value
            return func(self, **processed_kwargs)
        return _wrap
    return _decorator


class ConfigurationCenter(object):

    FILE_REL_PATH = "weibo_crawl/config.ini"
    INDEX_KEY = 'app_id'

    @classmethod
    def _build_index(cls):
        cls._index = {}
        for sec_name, sec_obj in cls._config.items():
            app_id = sec_obj.get(cls.INDEX_KEY)
            if app_id:
                # add only if app_id is valid.
                cls._index[app_id] = sec_obj

    @classmethod
    def load_configuration(cls):
        cls._config = configparser.ConfigParser()
        with open(cls.FILE_REL_PATH) as f:
            cls._config.read_file(f)
        cls._build_index()

    @classmethod
    def _write_configuration_to_file(cls):
        with open(cls.FILE_REL_PATH, 'w') as f:
            cls._config.write(f)

    @classmethod
    def generate_app_infos(cls):
        for sec_obj in cls._index.values():
            yield dict(sec_obj.items())

    @classmethod
    @week_filter(
        'app_id', 'app_secret', 'redirect_uri',
        'code', 'access_token', 'username', 'password',
    )
    def update_by_appid(cls, app_id, **kwargs):
        sec_obj = cls._index[app_id]
        sec_obj.update(kwargs)
        cls._write_configuration_to_file()
