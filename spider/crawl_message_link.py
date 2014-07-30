"""
This program would crawl sitemaps pointed by http://weibo.com/sitemap.xml.
"""

from __future__ import unicode_literals

import os
import xml.etree.ElementTree as ET
import requests


XML_PREFIX = "{http://www.sitemaps.org/schemas/sitemap/0.9}"
tag_adaptor = lambda x: XML_PREFIX + x


class SitemapSpider(object):

    @classmethod
    def get_content(cls, url):
        """
        @brief: GET url.
        @return: body of response.
        """
        print "Request: ", url
        response = requests.get(url)
        response.raise_for_status()
        return response.text

    @classmethod
    def sitemap_link_generator(cls, doc):
        """
        @return: date in format "yyyymmdd", url of sitemap.
        """
        root = ET.fromstring(doc)
        for sitemap_node in root.iter(tag_adaptor('sitemap')):
            date = sitemap_node.find(tag_adaptor('lastmod'))
            url = sitemap_node.find(tag_adaptor('loc'))
            yield date.text, url.text

    @classmethod
    def message_link_generator(cls, doc):
        """
        @return: url of message.
        """
        root = ET.fromstring(doc)
        for url_node in root.iter('loc'):
            yield url_node.text.replace(".com", ".cn")

    @classmethod
    def message_link_checker(cls, url):
        """
        @brief: Check url is valid or not.
        """
        if "cn//" in url:
            return False
        return True


class SitemapWriter(object):
    """
    @brief: Keep message urls extracted from sitemap. Currently the urls are
            directly written to files, might change in the future.
    """

    @classmethod
    def _message_file_path(cls, date, url):
        xml_file_name = url.rpartition("/")[-1]
        message_file_name = "{}-{}".format(date, xml_file_name)
        base_path = os.path.join(os.getcwd(), 'sitemaps')
        return os.path.join(base_path, message_file_name)

    @classmethod
    def should_update(cls, date, url):
        file_path = cls._message_file_path(date, url)
        if os.path.exists(file_path):
            return False
        return True

    @classmethod
    def write_message(cls, date, url, sitemap_doc):
        file_path = cls._message_file_path(date, url)
        print "Begin ", file_path
        with open(file_path, "w") as output:
            content = ",".join(
                filter(SitemapSpider.message_link_checker,
                       SitemapSpider.message_link_generator(sitemap_doc)),
            )
            output.write(content)
        print "Finish ", file_path


def main():
    top_level_sitemap_url = "http://weibo.com/sitemap.xml"
    top_level_sitemap_doc = SitemapSpider.get_content(top_level_sitemap_url)

    sitemap_gen = SitemapSpider.sitemap_link_generator(top_level_sitemap_doc)
    for date, sitemap_url in sitemap_gen:
        if SitemapWriter.should_update(date, sitemap_url):
            # make request of sitemap.
            sitemap_doc = SitemapSpider.get_content(sitemap_url)
            SitemapWriter.write_message(date, sitemap_url, sitemap_doc)


if __name__ == "__main__":
    main()
