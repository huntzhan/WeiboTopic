
# coding=utf-8

from __future__ import unicode_literals

import requests
from lxml.html import fromstring, tostring, submit_form

ENCODING = 'UTF-8'


def make_request():
    login_url = "http://login.weibo.cn/login/"
    response = requests.get(login_url)
    print response

    html_tree = fromstring(response.text.encode(ENCODING))
    form = html_tree.forms[0]
    # extract password field.
    password_field = None
    username_field = b"mobile"
    for key in form.fields.keys():
        if "password" in key:
            password_field = key
            break

    form.fields[username_field] = "programmer.zhx@gmail.com"
    form.fields[password_field] = "huntzhan"

    # fill form.
    encode = lambda x: x.encode(ENCODING)

    login_form = {}
    for key, value in form.form_values():
        print key, value
        login_form[encode(key)] = encode(value)
    # login_form['submit'] = encode("登录")
    form.fields = login_form

    print "####################"
    print login_form
    print login_url + form.action,
    print response.cookies
    print "####################"
    # login_response = requests.post(
    #     login_url,
    #     data=login_form,
    #     headers=response.headers,
    #     cookies=response.cookies,
    # )
    login_response = submit_form(form)
    return login_response

r = make_request()
print r.headers
print r.text
