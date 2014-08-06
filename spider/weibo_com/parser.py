from __future__ import (unicode_literals, print_function, absolute_import)

import re
import json
import urllib

from .utils import beautiful_soup, urldecode


#############
# from cola #
#############
class FriendPageParser(object):

    def _gen_internal_variables(self, requested_url, response_content):
        """
        @return: html, decodes, is_follow, is_new_mode.
        """

        soup = beautiful_soup(response_content)

        html = None
        decodes = urldecode(requested_url)
        is_follow = True
        is_new_mode = False
        for script in soup.find_all('script'):
            text = script.text
            if text.startswith('FM.view'):
                text = text.strip().replace(
                    ';', '').replace(
                    'FM.view(', '')[
                    :-1]
                data = None
                try:
                    data = json.loads(text)
                except ValueError as e:
                    # return self._error(url, e)
                    return None

                domid = data['domid']
                if domid.startswith('Pl_Official_LeftHisRelation__'):
                    html = beautiful_soup(data['html'])
                if 'relate' in decodes and decodes['relate'] == 'fans':
                    is_follow = False
                is_new_mode = True
            elif 'STK' in text:
                text = text.replace(
                    'STK && STK.pageletM && STK.pageletM.view(', '')[
                    :-1]
                data = json.loads(text)
                if data['pid'] == 'pl_relation_hisFollow' or \
                        data['pid'] == 'pl_relation_hisFans':
                    html = beautiful_soup(data['html'])
                if data['pid'] == 'pl_relation_hisFans':
                    is_follow = False

        return html, decodes, is_follow, is_new_mode

    def _extract_uids(self, html, decodes):
        """
        @return: new uids.
        """
        new_uids = []

        ul = None
        try:
            ul = html.find(
                attrs={
                    'class': 'cnfList',
                    'node-type': 'userListBox',
                },
            )
        except AttributeError as e:
            return None
        if ul is None:
            return None

        for li in ul.find_all(attrs={'class': 'S_line1',
                                     'action-type': 'itemClick'}):
            # extract uids.
            data = dict([l.split('=') for l in li['action-data'].split('&')])
            new_uids.append(data['uid'])

        return new_uids

    def _gen_next_page(self, url, html, decodes):
        pages = html.find(
            'div',
            attrs={
                'class': 'W_pages',
                'node-type': 'pageList',
            },
        )
        if pages is not None:
            a = pages.find_all('a')
            if len(a) > 0:
                next_ = a[-1]
                if next_['class'] == ['W_btn_c']:
                    # limit page number.
                    cur_page_num = int(decodes.get('page', 1))
                    if cur_page_num >= 10:
                        return None

                    decodes['page'] = cur_page_num + 1
                    query_str = urllib.urlencode(decodes)
                    url = '%s?%s' % (url.split('?')[0], query_str)
                    return url
        return None

    def _gen_fans_page(self, requested_url, is_new_mode):
        """
        @return: url points to fans page of user.
        """

        match = re.match(r"http://weibo.com/(\d+)/", requested_url)
        if match is None:
            raise Exception("Can't get uid: " + requested_url)
        uid = match.group(1)

        if is_new_mode:
            url_template = 'http://weibo.com/{}/follow?relate=fans'
        else:
            url_template = 'http://weibo.com/{}/fans'
        return url_template.format(uid)

    def parse(self, requested_url, response_content):
        """
        @input: requested url, content page being loaded.
        @output: new_uids, list of new uids; next_page, url of next page, None
                 if there's no such page; fans_page, url of fans page, None if
                 requested_url points exactly to a fans page.
        """

        variables = self._gen_internal_variables(
            requested_url,
            response_content,
        )
        if variables is None:
            return None
        else:
            html, decodes, is_follow, is_new_mode = variables

        # extract uids in page.
        new_uids = self._extract_uids(html, decodes)
        # gen url of next page.
        next_page = self._gen_next_page(requested_url, html, decodes)
        # gen url of fans page if currently points to follows page.
        if is_follow:
            fans_page = self._gen_fans_page(requested_url, is_new_mode)
        else:
            fans_page = None

        return new_uids, next_page, fans_page
