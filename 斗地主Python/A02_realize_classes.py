#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/7/30 16:43
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : A02_realize_classes.py
# @Software: PyCharm
import random

POKER_COLOR = ['方片♦', '梅花♣', '红心♥', '黑桃♠']
POKER_NUMBER = ['A'] + [str(i) for i in range(2, 11)] + ['J', 'Q', 'K']
POKER_KING = ['小王', '大王']
POKER_SIZE = {
    '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, '10': 10,
    'J': 11, 'Q': 12, 'K': 13, 'A': 14, '2': 15, '小王': 16, '大王': 17,
    '': 0, '方片♦': 0.1, '梅花♣': 0.2, '红心♥': 0.3, '黑桃♠': 0.4
}


class Poker(object):
    """
    A single poker.
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
        Str method of Poker.
        :return: color + number.
        """
        return self.__color + self.__number

    def __lt__(self, other):
        """
        Override < operator of Poker.
        :param other: another poker.
        :return: whether self is smaller than other.
        """
        left = POKER_SIZE[self.__color] + POKER_SIZE[self.__number]
        right = POKER_SIZE[other.__color] + POKER_SIZE[other.__number]
        if left < right:
            return True
        else:
            return False


class Pokers(object):
    """
    A series of pokers.
    """

    def __init__(self):
        """
        Init pokers.
        """
        self._pokers = []

    def __str__(self):
        """
        Str method of Pokers.
        :return: all pokers.
        """
        pokers = []
        for poker in self._pokers:
            pokers.append(str(poker))

        return str(pokers)

    def sortPokers(self):
        self._pokers.sort(reverse=True)

    def getType(self):
        pass


class PokerStack(Pokers):
    """
    A stack of pokers.
    """

    def __init__(self):
        """
        Init a poker stack.
        """
        super().__init__()

        self.initPokerStack()
        self.shufflePokerStack()

    def initPokerStack(self):
        """
        Init a poker stack.
        :return: None
        """
        for color in POKER_COLOR:
            for number in POKER_NUMBER:
                poker = Poker(color, number)
                self._pokers.append(poker)

        for king in POKER_KING:
            poker = Poker(king, '')
            self._pokers.append(poker)

    def shufflePokerStack(self):
        """
        Shuffle the pokers.
        :return: None
        """
        # random.seed(20210730)
        random.shuffle(self._pokers)

    def getPokers(self, n=1):
        """
        Get n pokers from Pokers.
        :param n: number of pokers to get.
        :return: pokers.
        """
        pokers = []
        for _ in range(n):
            if self._pokers:
                pokers.append(self._pokers.pop())
            else:
                print('Error 01')

        return pokers


class SimplePlayer(Pokers):
    """
    A simple player inheritance from Pokers.
    """

    def __init__(self, id_, pokers: PokerStack):
        """
        Init a simple player.
        :param id_: player's id.
        :param pokers: a poker stack.
        """
        super().__init__()

        self.__id = id_
        self._pokers = pokers.getPokers(17)
        self.sortPokers()


class SimpleLandlordPoker(Pokers):
    """
    A simple landlord poker inheritance from Pokers.
    """

    def __init__(self, id_, pokers: PokerStack):
        """
        Init a simple landlord poker.
        :param id_: player's id.
        :param pokers: a poker stack.
        """
        super().__init__()

        self.__id = id_
        self._pokers = pokers.getPokers(3)


def main():
    poker_stack = PokerStack()
    print(poker_stack)

    player_a = SimplePlayer(0, poker_stack)
    player_b = SimplePlayer(1, poker_stack)
    player_c = SimplePlayer(2, poker_stack)
    landlord_three = SimpleLandlordPoker(3, poker_stack)

    print(player_a)
    print(player_b)
    print(player_c)
    print(landlord_three)

    print(poker_stack)
    pass


if __name__ == '__main__':
    main()
