
import ConfigParser


def read_config():
    config = ConfigParser.ConfigParser()
    try:
        config.read("weibo_crawl/config.ini")
    except:
        raise Exception("fail to read config.ini")

    # read config options form sections
    res = []
    for sect in config.sections():
        values = {}
        options = config.options(sect)
        for option in options:
            try:
                values[option] = config.get(sect, option)
            except:
                raise Exception("fail to read {0} in section {1} \
                                in config.ini".format(
                                option,
                                sect))
        res.append(values)
    return res
