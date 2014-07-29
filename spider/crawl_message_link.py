"""
This program would crawl sitemaps pointed by http://weibo.com/sitemap.xml.
"""

from __future__ import unicode_literals

import os
import xml.etree.ElementTree as ET
import requests

XML_PREFIX = "{http://www.sitemaps.org/schemas/sitemap/0.9}"
tag_adaptor = lambda x: XML_PREFIX + x


def sitemap_link_generator(doc):
    """
    @return: date in format "yyyymmdd", url of sitemap.
    """
    root = ET.fromstring(doc)
    for sitemap_node in root.iter(tag_adaptor('sitemap')):
        date = sitemap_node.find(tag_adaptor('lastmod'))
        url = sitemap_node.find(tag_adaptor('loc'))
        yield date.text, url.text


def message_link_generator(doc):
    """
    @return: url of message.
    """
    root = ET.fromstring(doc)
    for url_node in root.iter('loc'):
        yield url_node.text.replace(".com", ".cn")


def message_link_checker(url):
    """
    Check url is valid or not.
    """
    if "cn//" in url:
        return False
    return True


def get_message_file_path(date, url):
    xml_file_name = url.rpartition("/")[-1]
    message_file_name = "{}-{}".format(date, xml_file_name)
    return os.path.join(os.getcwd(), message_file_name)


def main():
    top_level_sitemap_url = "http://weibo.com/sitemap.xml"
    response = requests.get(top_level_sitemap_url)
    top_level_sitemap_doc = response.text

    for date, sitemap_url in sitemap_link_generator(top_level_sitemap_doc):
        file_path = get_message_file_path(date, sitemap_url)
        if os.path.exists(file_path):
            continue
        # make request of sitemap.
        sitemap_doc = requests.get(sitemap_url).text
        print "Begin ", file_path
        with open(file_path, "w") as output:
            output.write(
                ",".join(filter(message_link_checker,
                                message_link_generator(sitemap_doc))),
            )
        print "Finish ", file_path


if __name__ == "__main__":
    main()
