
import ConfigParser

section_db = 'DB'


class Config:
    config = ConfigParser.ConfigParser()
    try:
        config.read("weibo_com/config.ini")
    except:
        raise Exception("fail to read config.ini")

    # read config options form 'DB' section
    sects = config.sections()
    if section_db not in sects:
        raise Exception('No "%s" section in config.ini', section_db)

    values = {}
    options = config.options(section_db)
    for option in options:
        try:
            values[option] = config.get(section_db, option).strip()
        except:
            raise Exception("fail to read {0} in section {1} \
                            in config.ini".format(
                            option,
                            section_db))
