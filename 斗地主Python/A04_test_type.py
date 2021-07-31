#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/7/31 19:59
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : A04_test_type.py
# @Software: PyCharm
from A02_realize_classes import *


def testPokerType():
    # 3
    poker = Pokers([Poker('♦', '3')])
    print(poker.getType())

    # 33
    poker = Pokers([Poker('♦', '3'), Poker('♣', '3')])
    print(poker.getType())

    # 34
    poker = Pokers([Poker('♦', '3'), Poker('♣', '4')])
    print(poker.getType())

    # joker, JOKER
    poker = Pokers([Poker('', 'joker'), Poker('', 'JOKER')])
    print(poker.getType())

    # 333
    poker = Pokers([Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3')])
    print(poker.getType())

    # 336
    poker = Pokers([Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '6')])
    print(poker.getType())

    # 3333
    poker = Pokers([Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '3')])
    print(poker.getType())

    # 3334
    poker = Pokers([Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '4')])
    print(poker.getType())

    # 6663
    poker = Pokers([Poker('♦', '6'), Poker('♥', '6'), Poker('♣', '6'), Poker('♠', '3')])
    print(poker.getType())

    # 6653
    poker = Pokers([Poker('♦', '6'), Poker('♥', '6'), Poker('♣', '5'), Poker('♠', '3')])
    print(poker.getType())

    # 33334
    poker = Pokers([Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '3'), Poker('♠', '4')])
    print(poker.getType())

    # 33344
    poker = Pokers([Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '4'), Poker('♠', '4')])
    print(poker.getType())

    # 66633
    poker = Pokers([Poker('♦', '6'), Poker('♥', '6'), Poker('♣', '6'), Poker('♠', '3'), Poker('♦', '3')])
    print(poker.getType())

    # 34567
    poker = Pokers([Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7')])
    print(poker.getType())

    # 34568
    poker = Pokers([Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '8')])
    print(poker.getType())

    # 334455
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '4'), Poker('♠', '4'), Poker('♠', '5'), Poker('♣', '5')])
    print(poker.getType())

    # 333355
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '3'), Poker('♠', '5'), Poker('♣', '5')])
    print(poker.getType())

    # AAAA55
    poker = Pokers(
        [Poker('♦', 'A'), Poker('♥', 'A'), Poker('♣', 'A'), Poker('♠', 'A'), Poker('♠', '5'), Poker('♣', '5')])
    print(poker.getType())

    # 333444
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '4'), Poker('♦', '4'), Poker('♣', '4')])
    print(poker.getType())

    # 333555
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '5'), Poker('♦', '5'), Poker('♣', '5')])
    print(poker.getType())

    # 345678
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7'), Poker('♠', '8')])
    print(poker.getType())

    # 345679
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7'), Poker('♠', '9')])
    print(poker.getType())

    # 33334444
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '3'),
         Poker('♦', '4'), Poker('♥', '4'), Poker('♣', '4'), Poker('♠', '4')])
    print(poker.getType())

    # 33335555
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '3'),
         Poker('♦', '5'), Poker('♥', '5'), Poker('♣', '5'), Poker('♠', '5')])
    print(poker.getType())

    # 33344456
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '4'), Poker('♦', '4'), Poker('♥', '4'),
         Poker('♣', '5'), Poker('♠', '6')])
    print(poker.getType())

    # 33377756
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '3'), Poker('♠', '7'), Poker('♦', '7'), Poker('♥', '7'),
         Poker('♣', '5'), Poker('♠', '6')])
    print(poker.getType())

    # 33445566
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '4'), Poker('♠', '4'), Poker('♠', '5'), Poker('♣', '5'),
         Poker('♠', '6'), Poker('♣', '6')])
    print(poker.getType())

    # 33445577
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '3'), Poker('♣', '4'), Poker('♠', '4'), Poker('♠', '5'), Poker('♣', '5'),
         Poker('♠', '7'), Poker('♣', '7')])
    print(poker.getType())

    # 345678910
    poker = Pokers(
        [Poker('♦', '3'), Poker('♥', '4'), Poker('♣', '5'), Poker('♠', '6'), Poker('♠', '7'), Poker('♠', '8'),
         Poker('♠', '9'), Poker('♠', '10')])
    print(poker.getType())
    pass


if __name__ == '__main__':
    testPokerType()
