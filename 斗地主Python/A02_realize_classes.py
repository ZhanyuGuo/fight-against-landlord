#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/7/30 16:43
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : A02_realize_classes.py
# @Software: PyCharm
import random
from enum import Enum

POKER_COLOR = ['♦', '♣', '♥', '♠']
POKER_NUMBER = ['A'] + [str(i) for i in range(2, 11)] + ['J', 'Q', 'K']
POKER_KING = ['joker', 'JOKER']

POKER_VALUE = {
    '3': 3, '4': 4, '5': 5, '6': 6, '7': 7, '8': 8, '9': 9, '10': 10,
    'J': 11, 'Q': 12, 'K': 13, 'A': 14, '2': 15, 'joker': 16, 'JOKER': 17,
    '': 0, '♦': 0.1, '♣': 0.2, '♥': 0.3, '♠': 0.4
}


class PokerType(Enum):
    none_type = 0
    single = 1
    double = 2
    triple = 3
    triple_one = 4
    triple_two = 5
    quadruple_two = 6
    continuous_single = 7
    continuous_double = 8
    plane = 9
    single_boom = 10
    double_boom = 11
    joker_boom = 12


class Poker(object):
    """
    A single poker.
    """

    def __init__(self, color: str, number: str):
        """
        Init a poker.

        :param color: color of the poker.
        :param number: number of the poker.
        """
        self.__color = color
        self.__number = number
        self.__value = POKER_VALUE[color] + POKER_VALUE[number]

    def getColor(self) -> str:
        """
        Get poker's color.

        :return: color.
        """
        return self.__color

    def getNum(self) -> str:
        """
        Get poker's number.

        :return: number.
        """
        return self.__number

    def __str__(self) -> str:
        """
        Str method of Poker.

        :return: color + number.
        """
        return self.__color + self.__number

    def __lt__(self, other) -> bool:
        """
        Override < operator of Poker.

        :param other: another poker.
        :return: whether self is smaller than other.
        """
        return self.__value < other.__value


class Pokers(object):
    """
    A series of poker.
    """

    def __init__(self, pokers=None):
        """
        Init a series of poker.

        :param pokers: a list of Poker.
        """
        if pokers is None:
            pokers = []

        self._pokers = pokers
        self.format_pokers = []
        self._type = None

    def __str__(self) -> str:
        """
        Str method of Pokers.

        :return: all pokers.
        """
        pokers = []
        for poker in self._pokers:
            pokers.append(str(poker))
        return str(pokers)

    def _sortPokers(self):
        """
        Sort the pokers according to color and number.

        :return: None
        """
        self._pokers.sort(reverse=False)
        self._formatPokers()

    def _formatPokers(self):
        """
        Format the pokers.

        :return: None
        """
        self.format_pokers = self._pokers.copy()
        for idx, poker in enumerate(self._pokers):
            self.format_pokers[idx] = POKER_VALUE[poker.getNum()]

    def _isContinuous(self) -> bool:
        """
        Judge continuity.

        :return: whether keys are continuous.
        """
        keys = sorted(self.getRepeatDict().keys())
        dif = keys[-1] - keys[0]
        return dif == len(keys) - 1

    def getRepeatDict(self) -> dict:
        """
        Get a dictionary of poker repetition.

        :return: a frequency dictionary.
        """
        tmp_dict = {}
        for poker in self.format_pokers:
            tmp_dict[poker] = tmp_dict.get(poker, 0) + 1
        return tmp_dict

    def getType(self):
        """
        Get pokers type, and set pokers type.

        :return: pokers type.
        """
        self._sortPokers()

        length = len(self.format_pokers)
        repeat_dict = self.getRepeatDict()

        repeat_values = repeat_dict.values()
        repeat_items = repeat_dict.items()

        sorted_repeat_values = sorted(repeat_values, reverse=True)
        sorted_repeat_items = sorted(repeat_items, key=lambda x: x[1], reverse=True)

        if length == 1:
            self._type = PokerType.single
        elif length == 2:
            if sorted_repeat_values[0] == 2:
                self._type = PokerType.double
            elif self.format_pokers[0] == 16 and self.format_pokers[1] == 17:
                self._type = PokerType.joker_boom
            else:
                self._type = PokerType.none_type
        elif length == 3:
            if sorted_repeat_values[0] == 3:
                self._type = PokerType.triple
            else:
                self._type = PokerType.none_type
        elif length == 4:
            if sorted_repeat_values[0] == 4:
                self._type = PokerType.single_boom
            elif sorted_repeat_values[0] == 3:
                self._type = PokerType.triple_one
            else:
                self._type = PokerType.none_type
        elif length == 5:
            if sorted_repeat_values[0] == 4:
                self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 3 and sorted_repeat_values[1] == 2:
                self._type = PokerType.triple_two
            elif sorted_repeat_values[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuous_single
                else:
                    self._type = PokerType.none_type
            else:
                self._type = PokerType.none_type
        elif length == 6:
            if sorted_repeat_values[0] == 4 and sorted_repeat_values[1] == 2:
                self._type = PokerType.quadruple_two
            elif sorted_repeat_values[0] == 3 and sorted_repeat_values[1] == 3:
                if self._isContinuous():
                    self._type = PokerType.plane
                else:
                    self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 2 and sorted_repeat_values[1] == 2 and sorted_repeat_values[2] == 2:
                if self._isContinuous():
                    self._type = PokerType.continuous_double
                else:
                    self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuous_single
                else:
                    self._type = PokerType.none_type
            else:
                self._type = PokerType.none_type
        elif length == 7:
            if sorted_repeat_values[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuous_single
                else:
                    self._type = PokerType.none_type
            else:
                self._type = PokerType.none_type
        elif length == 8:
            if sorted_repeat_values[0] == 4 and sorted_repeat_values[1] == 4:
                if self._isContinuous():
                    self._type = PokerType.double_boom
                else:
                    self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 3 and sorted_repeat_values[1] == 3 and sorted_repeat_values[2] == 1:
                if abs(sorted_repeat_items[0][0] - sorted_repeat_items[1][0]) == 1:
                    self._type = PokerType.plane
                else:
                    self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 2 and sorted_repeat_values[1] == 2 and sorted_repeat_values[2] == 2 and \
                    sorted_repeat_values[3] == 2:
                if self._isContinuous():
                    self._type = PokerType.continuous_double
                else:
                    self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuous_single
                else:
                    self._type = PokerType.none_type
            else:
                self._type = PokerType.none_type

        return self._type

    def __gt__(self, other) -> bool:
        """
        Override > operation of Pokers.
        
        :param other: another pokers.
        :return: whether self > other.
        """
        left_type = self.getType()
        right_type = other.getType()

        if right_type == PokerType.single:
            if left_type == PokerType.single:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.double:
            if left_type == PokerType.double:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.triple:
            if left_type == PokerType.triple:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.triple_one:
            if left_type == PokerType.triple_one:
                return self._compareMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.triple_two:
            if left_type == PokerType.triple_two:
                return self._compareMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.quadruple_two:
            if left_type == PokerType.quadruple_two:
                return self._compareMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.continuous_single:
            if left_type == PokerType.continuous_single:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.continuous_double:
            if left_type == PokerType.continuous_double:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.plane:
            # todo: compare two plane
            if left_type == PokerType.plane:
                pass
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.single_boom:
            if left_type == PokerType.single_boom:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            elif left_type == PokerType.joker_boom or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.joker_boom:
            if left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.double_boom:
            if left_type == PokerType.double_boom:
                return self._compareFirstValue(self.format_pokers, other.format_pokers)
            else:
                return False
            pass

        pass

    @staticmethod
    def _compareFirstValue(left, right):
        return left[0] > right[0]

    def _compareMostValue(self, other):
        left_dict = self.getRepeatDict()
        right_dict = other.getRepeatDict()

        left_item = left_dict.items()
        right_item = right_dict.items()

        sorted_left_item = sorted(left_item, key=lambda x: x[1], reverse=True)
        sorted_right_item = sorted(right_item, key=lambda x: x[1], reverse=True)

        return sorted_left_item[0][0] > sorted_right_item[0][0]

    def _compareFirstMostValue(self, other):
        # todo: first most value.
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
        self.__initPokerStack()
        self.__shufflePokerStack()

    def __initPokerStack(self):
        """
        Init a poker stack.

        :return: None.
        """
        for color in POKER_COLOR:
            for number in POKER_NUMBER:
                poker = Poker(color, number)
                self._pokers.append(poker)

        for king in POKER_KING:
            poker = Poker('', king)
            self._pokers.append(poker)

    def __shufflePokerStack(self):
        """
        Shuffle the pokers.

        :return: None.
        """
        # random.seed(20210730)
        random.shuffle(self._pokers)

    def getPokers(self, n=1) -> list:
        """
        Get n pokers from PokerStack.

        :param n: number of pokers to get.
        :return: a list of pokers.
        """
        pokers = []
        for _ in range(n):
            if self._pokers:
                pokers.append(self._pokers.pop())
            else:
                raise ValueError('[Error 01] Stack empty.')

        return pokers


class SimplePlayer(Pokers):
    """
    A simple player.
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
        self._sortPokers()


class SimpleLandlordPoker(Pokers):
    """
    A simple landlord poker.
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
