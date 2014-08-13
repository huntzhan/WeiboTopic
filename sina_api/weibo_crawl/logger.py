"""
@brief: defines function to setup root logger.
"""

import logging


def setup_logging():
    format_template = (
        '%(asctime)s, %(name)s, %(thread)s, %(levelname)s\n'
        '%(message)s\n'
    )
    formatter = logging.Formatter(format_template)

    file_handler = logging.FileHandler('RUNTIME_LOG')
    file_handler.setFormatter(formatter)
    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(formatter)

    root_logger = logging.getLogger()
    root_logger.addHandler(file_handler)
    root_logger.addHandler(stream_handler)
