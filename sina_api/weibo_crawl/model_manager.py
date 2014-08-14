
from __future__ import (unicode_literals, print_function, absolute_import)

import time


class ModelManager(object):
    """
    @brief: 1. manage a mapping from time.struct_time to models.
            2. responsible for creating new module.
    """

    time_models_mapping = {}

    @classmethod
    def _round_to_hour(cls, timestamp):
        """
        @input: timestamp is a time.struct_time object.
        @output: a time.struct_time rounded from timestamp.
        """

        time_format = '%Y %m %d %H'
        rounded_time_string = time.strftime(time_format, timestamp)
        return time.strptime(rounded_time_string, time_format)

    @classmethod
    def _create_new_models(cls, rounded_timestamp):
        """
        @brief: convert time.struct_time to epoch, create models, keep mapping,
                then return newly created models.
        """

        suffix = str(
            int(time.mktime(rounded_timestamp)))
        models = create_model_class(suffix)
        cls.time_models_mapping[rounded_timestamp] = models
        return models

    @classmethod
    def get_models(cls, timestamp):
        rounded_timestamp = cls._round_to_hour(timestamp)
        models = cls.time_models_mapping.get(rounded_timestamp, None)
        if models:
            return models
        else:
            return cls._create_new_models(rounded_timestamp)
