#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/1 14:46
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : basic.py
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
    # to be perfect...
    none_type = 0
    single = 1
    double = 2
    triple = 3
    triple_one = 4
    triple_two = 5
    quadruple_double = 6
    # quadruple_two...
    continuous_single = 7
    continuous_double = 8
    plane = 9
    single_boom = 10
    double_boom = 11
    joker_boom = 12
    empty = 13


class Poker(object):
    def __init__(self, color: str, number: str):
        self.__color = color
        self.__number = number
        self.__value = POKER_VALUE[color] + POKER_VALUE[number]
        self.__value_no_color = POKER_VALUE[number]

    def getColor(self) -> str:
        return self.__color

    def getNumber(self) -> str:
        return self.__number

    def getValue(self) -> int:
        return self.__value

    def getValueNoColor(self) -> int:
        return self.__value_no_color

    def __str__(self) -> str:
        return self.__color + self.__number

    def __lt__(self, other) -> bool:
        return self.__value < other.__value


class Pokers(object):
    def __init__(self, pokers=None):
        if pokers is None:
            pokers = []
        self._pokers = pokers
        self._format_pokers = []
        self._repeat_dict = {}
        self._type = None

    def getPokers(self) -> list:
        return self._pokers

    def setPokers(self, poker_list: list) -> None:
        self._pokers = poker_list

    def extendPokers(self, poker_list: list) -> None:
        self._pokers.extend(poker_list)

    def getFormatPokers(self) -> list:
        return self._format_pokers

    def getRepeatDict(self) -> dict:
        return self._repeat_dict

    def getType(self) -> PokerType:
        return self._type

    def sortGetType(self) -> None:
        self.sortPokers()
        length = len(self._format_pokers)
        repeat_values = self._repeat_dict.values()
        repeat_items = self._repeat_dict.items()
        sorted_repeat_values = sorted(repeat_values, reverse=True)
        sorted_repeat_items = sorted(repeat_items, key=lambda x: x[1], reverse=True)

        if length == 0:
            self._type = PokerType.empty
        elif length == 1:
            self._type = PokerType.single
        elif length == 2:
            if sorted_repeat_values[0] == 2:
                self._type = PokerType.double
            elif self._format_pokers[0] == 16 and self._format_pokers[1] == 17:
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
            if sorted_repeat_values[0] == 3 and sorted_repeat_values[1] == 2:
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
                self._type = PokerType.quadruple_double
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
            elif sorted_repeat_values[0] == 3 and sorted_repeat_values[1] == 3 and \
                    (sorted_repeat_values[2] == 2 or sorted_repeat_values[2] == 1):
                if abs(sorted_repeat_items[0][0] - sorted_repeat_items[1][0]) == 1:
                    self._type = PokerType.plane
                else:
                    self._type = PokerType.none_type
            elif sorted_repeat_values[0] == 2 and sorted_repeat_values[1] == 2 and \
                    sorted_repeat_values[2] == 2 and sorted_repeat_values[3] == 2:
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

    def sortPokers(self) -> None:
        self._pokers.sort(reverse=False)
        self._formatPokers()
        self._repeatDict()

    def _formatPokers(self) -> None:
        self._format_pokers = self._pokers.copy()
        for idx, poker in enumerate(self._pokers):
            self._format_pokers[idx] = poker.getValueNoColor()

    def _repeatDict(self) -> None:
        self._repeat_dict = {}
        for poker in self._format_pokers:
            self._repeat_dict[poker] = self._repeat_dict.get(poker, 0) + 1

    def _isContinuous(self) -> bool:
        keys = sorted(self._repeat_dict.keys())
        if keys[-1] > 14:  # max poker > A
            return False

        dif = keys[-1] - keys[0]
        return dif == len(keys) - 1

    def __str__(self) -> str:
        pokers = []
        for poker in self._pokers:
            pokers.append(str(poker))
        return str(pokers)

    def __gt__(self, other) -> bool:
        self.sortGetType()
        other.sortGetType()
        left_type = self._type
        right_type = other._type

        if right_type == PokerType.empty:
            if left_type == PokerType.none_type:
                return False
            else:
                return True
        elif right_type == PokerType.single:
            if left_type == PokerType.single:
                return self._compareFirstMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.double:
            if left_type == PokerType.double:
                return self._compareFirstMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.triple:
            if left_type == PokerType.triple:
                return self._compareFirstMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.triple_one:
            if left_type == PokerType.triple_one:
                return self._compareFirstMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.triple_two:
            if left_type == PokerType.triple_two:
                return self._compareFirstMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.quadruple_double:
            if left_type == PokerType.quadruple_double:
                return self._compareFirstMostValue(other)
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.continuous_single:
            if left_type == PokerType.continuous_single:
                if len(self._format_pokers) == len(other._format_pokers):
                    return self._compareFirstMostValue(other)
                else:
                    return False
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.continuous_double:
            if left_type == PokerType.continuous_double:
                if len(self._format_pokers) == len(other._format_pokers):
                    return self._compareFirstMostValue(other)
                else:
                    return False
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.plane:
            if left_type == PokerType.plane:
                if len(self._format_pokers) == len(other._format_pokers):
                    return self._compareFirstMostValue(other)
                else:
                    return False
            elif left_type == PokerType.single_boom or left_type == PokerType.joker_boom \
                    or left_type == PokerType.double_boom:
                return True
            else:
                return False
        elif right_type == PokerType.single_boom:
            if left_type == PokerType.single_boom:
                return self._compareFirstMostValue(other)
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
                return self._compareFirstMostValue(other)
            else:
                return False

    def _compareFirstMostValue(self, other) -> bool:
        left_dict = self._repeat_dict
        right_dict = other._repeat_dict

        left_value = left_dict.values()
        left_item = left_dict.items()
        right_item = right_dict.items()

        most_count = max(left_value)
        left_most_count_key = [item[0] for item in left_item if item[1] == most_count]
        right_most_count_key = [item[0] for item in right_item if item[1] == most_count]

        left_most_count_key.sort()
        right_most_count_key.sort()

        return left_most_count_key[0] > right_most_count_key[0]


class PokerStack(Pokers):
    def __init__(self):
        super().__init__()
        self.__initPokerStack()
        self.__shufflePokerStack()

    def __initPokerStack(self) -> None:
        for color in POKER_COLOR:
            for number in POKER_NUMBER:
                poker = Poker(color, number)
                self._pokers.append(poker)

        for king in POKER_KING:
            poker = Poker('', king)
            self._pokers.append(poker)

    def __shufflePokerStack(self) -> None:
        # random.seed(20210730)
        random.shuffle(self._pokers)

    def getPokersFromStack(self, n: int = 1) -> list:
        pokers = []
        for _ in range(n):
            if self._pokers:
                pokers.append(self._pokers.pop())
            else:
                raise ValueError('[Error 01] Stack empty.')

        return pokers


class SimplePlayer(Pokers):
    def __init__(self, id_: int, pokers: PokerStack):
        super().__init__()
        self.__id = id_
        self._pokers = pokers.getPokersFromStack(17)
        self.sortPokers()

    def getId(self) -> int:
        return self.__id

    def showPokers(self) -> str:
        rlt = ''
        for idx, poker in enumerate(self._pokers):
            rlt += f"[{idx}]:{poker} "
        return rlt

    def outPokers(self, index: list, previous: Pokers) -> Pokers or None:
        index = list(set(index))
        index.sort(reverse=True)
        out = Pokers()
        for idx in index:
            out._pokers.append(self._pokers[idx])

        if out > previous:
            for idx in index:
                self._pokers.pop(idx)
            return out
        else:
            return None


class SimpleLandlordPokers(Pokers):
    def __init__(self, pokers: PokerStack):
        super().__init__()
        self._pokers = pokers.getPokersFromStack(3)


def main():
    poker_stack = PokerStack()

    player_a = SimplePlayer(0, poker_stack)
    player_b = SimplePlayer(1, poker_stack)
    player_c = SimplePlayer(2, poker_stack)
    landlord_three = SimpleLandlordPokers(poker_stack)

    print(player_a)
    print(player_b)
    print(player_c)
    print(landlord_three)

    print(player_a.showPokers())
    index = list(map(int, input('出牌：\r\n').split()))
    print(player_a.outPokers(index, Pokers()))
    print(player_a.showPokers())


if __name__ == '__main__':
    main()
