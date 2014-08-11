from __future__ import (unicode_literals, print_function, absolute_import)

import re
import json
import urllib
import time

from dateutil.parser import parse

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
        @return: current_uid; url points to fans page of user.
        """

        match = re.match(r"http://weibo.com/(\d+)/", requested_url)
        if match is None:
            raise Exception("Can't get uid: " + requested_url)
        uid = match.group(1)

        if is_new_mode:
            url_template = 'http://weibo.com/{}/follow?relate=fans'
        else:
            url_template = 'http://weibo.com/{}/fans'
        return uid, url_template.format(uid)

    def parse(self, requested_url, response_content):
        """
        @input: requested url, content page being loaded.
        @output: current_uid; new_uids, list of new uids; next_page, url of
                 next page, None if there's no such page; fans_page, url of
                 fans page, None if requested_url points exactly to a fans
                 page.
        """

        variables = self._gen_internal_variables(
            requested_url,
            response_content,
        )
        if variables is None:
            return None
        else:
            html, decodes, is_follow, is_new_mode = variables

        if html is None:
            print("HTML is None: ", requested_url)

        # extract uids in page.
        new_uids = self._extract_uids(html, decodes)
        # gen url of next page.
        next_page = self._gen_next_page(requested_url, html, decodes)
        # gen url of fans page if currently points to follows page.
        current_uid, fans_page = self._gen_fans_page(
            requested_url,
            is_new_mode,
        )
        if not is_follow:
            fans_page = None

        return current_uid, new_uids, next_page, fans_page


#############
# from cola #
#############
class MicroBlogParser(object):

    MAX_PAGE = 10

    def __init__(self, requested_url, response_content):
        self.requested_url = requested_url
        self.response_content = response_content

    def _preprocess_url(self, requested_url):
        params = urldecode(requested_url)

        params['_t'] = 0
        params['__rnd'] = str(int(time.time() * 1000))
        page = int(params.get('page', 1))
        pre_page = int(params.get('pre_page', 0))
        count = 15
        if 'pagebar' not in params:
            params['pagebar'] = '0'
            pre_page += 1
        elif params['pagebar'] == '0':
            params['pagebar'] = '1'
        elif params['pagebar'] == '1':
            del params['pagebar']
            pre_page = page
            page += 1
            count = 50
        params['count'] = count
        params['page'] = page
        params['pre_page'] = pre_page

        if page > self.MAX_PAGE:
            continue_flag = False
        else:
            continue_flag = True
        return params, continue_flag

    def _get_url_of_next_page(self, requested_url, params):
        url_of_next_page = '{}?{}'.format(
            requested_url.split('?')[0],
            urllib.urlencode(params),
        )
        return url_of_next_page

    def _data_generator(self, params, response_content):

        uid = params['uid']

        # data = json.loads(br.response().read())['data']
        data = json.loads(response_content)['data']
        soup = beautiful_soup(data)
        finished = False

        divs = soup.find_all('div', attrs={'class': 'WB_feed_type'},  mid=True)
        max_id = None
        next_urls = []

        #########################################
        # generate one micro blog on each loop. #
        #########################################
        for div in divs:
            # extract mid here!
            mid = div['mid']
            if len(mid) == 0:
                continue
            max_id = mid

            if 'end_id' not in params:
                params['end_id'] = mid

            content_div = div.find('div', attrs={
                'class': 'WB_text',
                'node-type': 'feed_list_content'
            })
            for img in content_div.find_all("img", attrs={'type': 'face'}):
                img.replace_with(img['title'])

            #########################
            # Extract content here. #
            #########################
            # mblog.content = content_div.text
            content = content_div.text

            ######################################################
            # Extract content from which the message forwarding. #
            ######################################################
            forwarded_content = None
            is_forward = div.get('isforward') == '1'
            if is_forward:

                ######################
                # Extract omid here. #
                ######################
                name_a = div.find('a', attrs={
                    'class': 'WB_name',
                    'node-type': 'feed_list_originNick'
                })
                text_a = div.find('div', attrs={
                    'class': 'WB_text',
                    'node-type': 'feed_list_reason'
                })

                if text_a:
                    forwarded_content = text_a.text

            ########################
            # Extract created time #
            ########################
            created = parse(div.select('a.S_link2.WB_time')[0]['title'])
            created_time = created.isoformat()

            func_div = div.find_all('div', 'WB_func')[-1]
            action_type_re = lambda t: re.compile("^(feed_list|fl)_%s$" % t)

            likes = func_div.find(
                'a', attrs={'action-type': action_type_re("like")}).text
            likes = likes.strip('(').strip(')')
            likes = 0 if len(likes) == 0 else int(likes)

            #################################################
            # Extract number of favorite on this micro blog #
            #################################################
            # mblog.n_likes = likes
            number_of_favourite = likes

            ################################################
            # Extract number of forward on this micro blog #
            ################################################
            forwards = func_div.find(
                'a', attrs={'action-type': action_type_re("forward")}).text

            if '(' not in forwards:
                number_of_forward = 0
            else:
                number_of_forward = int(
                    forwards.strip().split('(', 1)[1].strip(')'))

            #################################################
            # Extract number of comments on this micro blog #
            #################################################
            comments = func_div.find(
                'a', attrs={'action-type': action_type_re('comment')}).text

            if '(' not in comments:
                number_of_comment = 0
            else:
                number_of_comment = int(
                    comments.strip().split('(', 1)[1].strip(')'))

            ################
            # yield result #
            ################
            raw_message = (
                uid, mid,
                content, forwarded_content,
                created_time,
                number_of_favourite, number_of_comment,
                number_of_forward,
            )
            encode_item = lambda x: x.encode('utf-8')\
                if isinstance(x, unicode) else x
            processed_message = list(map(encode_item, raw_message))
            yield processed_message

        if 'pagebar' in params:
            params['max_id'] = max_id
        else:
            del params['max_id']

    def parse(self):
        """
        @input: requested url, content page being loaded.
        @output:
        """
        params, continue_flag = self._preprocess_url(self.requested_url)
        messages = []
        for message in self._data_generator(params, self.response_content):
            messages.append(message)

        url_of_next_page = self._get_url_of_next_page(
            self.requested_url,
            params,
        ) if continue_flag else None

        return messages, url_of_next_page
