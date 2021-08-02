#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/7/31 21:07
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : A05_test_comparison.py
# @Software: PyCharm
from A02_realize_classes import *


def testComparison():
    # poker_a = Pokers([Poker('♦', '3')])
    # poker_b = Pokers([Poker('♦', '4')])
    # print(poker_b > poker_a)
    # poker_b = Pokers([Poker('♦', '4'), Poker('♣', '4')])
    # print(poker_b > poker_a)
    #
    # poker_a = Pokers([Poker('♦', '3'), Poker('♣', '3')])
    # poker_b = Pokers([Poker('♦', '4'), Poker('♣', '4')])
    # print(poker_b > poker_a)
    # poker_b = Pokers([Poker('', 'joker'), Poker('', 'JOKER')])
    # print(poker_b > poker_a)
    # poker_b = Pokers([Poker('♦', '3'), Poker('♣', '4')])
    # print(poker_b > poker_a)
    #
    # poker_a = Pokers()
    # poker_b = Pokers([Poker('♦', '4'), Poker('♣', '4')])
    # print(poker_b > poker_a)
    #
    # poker_a = Pokers([Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7')])
    # poker_b = Pokers([Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7'), Poker('♦', '8')])
    # print(poker_b > poker_a)
    #
    # poker_a = Pokers([Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7')])
    # poker_b = Pokers(
    #     [Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7'), Poker('♦', '8'), Poker('♦', '9')])
    # print(poker_b > poker_a)
    poker_a = Pokers(
        [Poker('♦', '3'), Poker('♣', '3'), Poker('♥', '3'), Poker('♦', '4'), Poker('♣', '4'), Poker('♥', '4'),
         Poker('♦', '5'), Poker('♣', '6')])

    poker_b = Pokers(
        [Poker('♦', '7'), Poker('♣', '7'), Poker('♥', '7'), Poker('♦', '8'), Poker('♣', '8'), Poker('♥', '8'),
         Poker('♦', '5'), Poker('♣', '6')])
    print(poker_b > poker_a)
    pass


if __name__ == '__main__':
    testComparison()
