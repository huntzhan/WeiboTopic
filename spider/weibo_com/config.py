
import ConfigParser

section_db = 'DB'

class Config:
    config = ConfigParser.ConfigParser()
    config.read("weibo_com/config.ini")

    # read config options form 'DB' section
    sects = config.sections()
    if section_db not in sects:
        raise Exception('No "%s" section in config.ini', section_db)

    values = {}
    options = config.options(section_db)
    for option in options:
        try:
            values[option] = config.get(section_db, option)
        except:
            raise Exception("exception while reading config.ini on %s!" %
                            option)
