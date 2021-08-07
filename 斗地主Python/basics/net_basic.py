#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/7 14:52
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : net_basic.py
# @Software: PyCharm
from enum import IntEnum


class ReturnState(IntEnum):
    not_current = 0
    error_out = 1
    cannot_skip = 2
    can_skip = 3
    succeed_out = 4
    pass
