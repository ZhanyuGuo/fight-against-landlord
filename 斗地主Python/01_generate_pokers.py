#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/7/29 20:37
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : 01_generate_pokers.py
# @Software: PyCharm
import random


def shufflePokers(src_pokers):
    dst_pokers = src_pokers.copy()
    random.shuffle(dst_pokers)
    return dst_pokers


def main():
    poker_num = [str(i) for i in range(2, 11)]
    poker_str = ['A', 'J', 'Q', 'K']
    poker_king = ['大王', '小王']
    poker_color = ['方片♦', '梅花♣', '红心♥', '黑桃♠']
    pokers = ['%s%s' % (i, j) for i in poker_color for j in poker_num + poker_str] + poker_king
    pokers_shuffled = shufflePokers(pokers)
    print(pokers)
    print(pokers_shuffled)

    landlord_three = pokers_shuffled[:3]
    player_a = pokers_shuffled[3::3]
    player_b = pokers_shuffled[4::3]
    player_c = pokers_shuffled[5::3]

    print(landlord_three)
    print(player_a)
    print(player_b)
    print(player_c)


if __name__ == '__main__':
    main()
