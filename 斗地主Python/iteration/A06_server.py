#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/3 22:08
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : A06_server.py
# @Software: PyCharm
from flask import Flask
from markupsafe import escape
from basics.basic import *

app = Flask(__name__)


@app.route("/")
def index():
    return "Index Page"


@app.route("/hello")
def hello():
    return "hello world!"


@app.route("/<name>")
def showName(name):
    return f"Hello, {escape(name)}"


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
