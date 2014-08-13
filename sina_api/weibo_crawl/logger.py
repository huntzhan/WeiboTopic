"""
@brief: defines function to setup root logger.
"""

import logging


def setup_logging():
    format_template = (
        '%(asctime)s - %(name)s - %(thread)s- %(levelname)s\n'
        '%(message)s\n'
        '- end -'
    )

    logging.basicConfig(
        format=format_template,
        level=logging.INFO,
    )
