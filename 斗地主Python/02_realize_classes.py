#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/7/30 16:43
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : 02_realize_classes.py
# @Software: PyCharm
import random

POKER_COLOR = ['方片♦', '梅花♣', '红心♥', '黑桃♠']
POKER_NUMBER = ['A'] + [str(i) for i in range(2, 11)] + ['J', 'Q', 'K']
POKER_KING = ['小王', '大王']


class Poker(object):
    """
    A poker.
    """

    def __init__(self, color, number):
        """
        Init a poker.
        :param color: color of the poker.
        :param number: number of the poker.
        """
        self.__color = color
        self.__number = number

    def getColor(self):
        """
        Get poker's color.
        :return: color.
        """
        return self.__color

    def getNum(self):
        """
        Get poker's number.
        :return: number.
        """
        return self.__number

    def __str__(self):
        """
        Str method.
        :return: color + number.
        """
        return self.__color + self.__number


class Pokers(object):
    """
    A stack of pokers.
    """

    def __init__(self):
        """
        Init a poker stack.
        """
        self.__pokers = []
        self.initPokers()
        self.shufflePokers()

    def initPokers(self):
        """
        Init a poker stack.
        :return: None
        """
        for color in POKER_COLOR:
            for number in POKER_NUMBER:
                poker = Poker(color, number)
                self.__pokers.append(poker)

        for king in POKER_KING:
            poker = Poker(king, '')
            self.__pokers.append(poker)

    def shufflePokers(self):
        """
        Shuffle the pokers.
        :return: None
        """
        random.seed(20210730)
        random.shuffle(self.__pokers)

    def getPokers(self, n=1):
        """
        Get n pokers from Pokers.
        :param n: number of pokers to get.
        :return: pokers.
        """
        pokers = []
        for _ in range(n):
            if self.__pokers:
                pokers.append(self.__pokers.pop())
            else:
                print('Error 01')

        return pokers

    def __str__(self):
        """
        Str method.
        :return: all pokers.
        """
        lst = []
        for poker in self.__pokers:
            lst.append(str(poker))

        return str(lst)


class SimplePlayer(object):
    def __init__(self, id_, pokers: Pokers):
        self.__id = id_
        self.__pokers = pokers.getPokers(17)
        pass

    def __str__(self):
        lst = []
        for poker in self.__pokers:
            lst.append(str(poker))

        return str(lst)

    pass


def printPokers(player):
    for poker in player:
        print(poker, end=' ')
    print(len(player))


def main():
    # print(POKER_COLOR)
    # print(POKER_NUMBER)
    # print(POKER_KING)

    # p = Poker('方片♦', 'A')
    # print(p)

    P = Pokers()
    print(P)

    # player_a = P.getPokers(17)
    # player_b = P.getPokers(17)
    # player_c = P.getPokers(17)
    # landlord_three = P.getPokers(3)
    #
    # printPokers(player_a)
    # printPokers(player_b)
    # printPokers(player_c)
    # printPokers(landlord_three)
    #
    # P.getPokers()

    player_a = SimplePlayer(0, P)
    player_b = SimplePlayer(1, P)
    player_c = SimplePlayer(2, P)

    print(player_a)
    print(player_b)
    print(player_c)

    print(P)
    pass


if __name__ == '__main__':
    main()
