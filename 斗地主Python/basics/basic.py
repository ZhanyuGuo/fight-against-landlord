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
    noneType = 0
    single = 1
    double = 2
    triple = 3
    tripleOne = 4
    tripleTwo = 5
    quadrupleDouble = 6
    # quadrupleTwo...
    continuousSingle = 7
    continuousDouble = 8
    plane = 9
    singleBoom = 10
    doubleBoom = 11
    jokerBoom = 12
    empty = 13


class ReturnState(Enum):
    notCurrent = 0
    errorOut = 1
    cannotSkip = 2
    canSkip = 3
    succeedOut = 4


class Poker(object):
    def __init__(self, color: str, number: str):
        self.__color = color
        self.__number = number
        self.__value = POKER_VALUE[color] + POKER_VALUE[number]
        self.__valueNoColor = POKER_VALUE[number]

    def getColor(self) -> str:
        return self.__color

    def getNumber(self) -> str:
        return self.__number

    def getValue(self) -> int:
        return self.__value

    def getValueNoColor(self) -> int:
        return self.__valueNoColor

    def __str__(self) -> str:
        return self.__color + self.__number

    def __lt__(self, other) -> bool:
        return self.__value < other.__value


class Pokers(object):
    def __init__(self, pokers=None):
        if pokers is None:
            pokers = []
        self._pokers = pokers
        self._formatPokers = []
        self._repeatDict = {}
        self._type = None

    def getPokers(self) -> list:
        return self._pokers

    def setPokers(self, poker_list: list) -> None:
        self._pokers = poker_list

    def extendPokers(self, poker_list: list) -> None:
        self._pokers.extend(poker_list)

    def getFormatPokers(self) -> list:
        return self._formatPokers

    def getRepeatDict(self) -> dict:
        return self._repeatDict

    def getType(self) -> PokerType:
        return self._type

    def sortAndFetchType(self) -> None:
        self.sortPokers()
        length = len(self._formatPokers)
        repeatValues = self._repeatDict.values()
        repeatItems = self._repeatDict.items()
        sortedRepeatValues = sorted(repeatValues, reverse=True)
        sortedRepeatItems = sorted(repeatItems, key=lambda x: x[1], reverse=True)

        if length == 0:
            self._type = PokerType.empty
        elif length == 1:
            self._type = PokerType.single
        elif length == 2:
            if sortedRepeatValues[0] == 2:
                self._type = PokerType.double
            elif self._formatPokers[0] == 16 and self._formatPokers[1] == 17:
                self._type = PokerType.jokerBoom
            else:
                self._type = PokerType.noneType
        elif length == 3:
            if sortedRepeatValues[0] == 3:
                self._type = PokerType.triple
            else:
                self._type = PokerType.noneType
        elif length == 4:
            if sortedRepeatValues[0] == 4:
                self._type = PokerType.singleBoom
            elif sortedRepeatValues[0] == 3:
                self._type = PokerType.tripleOne
            else:
                self._type = PokerType.noneType
        elif length == 5:
            if sortedRepeatValues[0] == 3 and sortedRepeatValues[1] == 2:
                self._type = PokerType.tripleTwo
            elif sortedRepeatValues[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuousSingle
                else:
                    self._type = PokerType.noneType
            else:
                self._type = PokerType.noneType
        elif length == 6:
            if sortedRepeatValues[0] == 4 and sortedRepeatValues[1] == 2:
                self._type = PokerType.quadrupleDouble
            elif sortedRepeatValues[0] == 3 and sortedRepeatValues[1] == 3:
                if self._isContinuous():
                    self._type = PokerType.plane
                else:
                    self._type = PokerType.noneType
            elif sortedRepeatValues[0] == 2 and sortedRepeatValues[1] == 2 and sortedRepeatValues[2] == 2:
                if self._isContinuous():
                    self._type = PokerType.continuousDouble
                else:
                    self._type = PokerType.noneType
            elif sortedRepeatValues[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuousSingle
                else:
                    self._type = PokerType.noneType
            else:
                self._type = PokerType.noneType
        elif length == 7:
            if sortedRepeatValues[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuousSingle
                else:
                    self._type = PokerType.noneType
            else:
                self._type = PokerType.noneType
        elif length == 8:
            if sortedRepeatValues[0] == 4 and sortedRepeatValues[1] == 4:
                if self._isContinuous():
                    self._type = PokerType.doubleBoom
                else:
                    self._type = PokerType.noneType
            elif sortedRepeatValues[0] == 3 and sortedRepeatValues[1] == 3 and \
                    (sortedRepeatValues[2] == 2 or sortedRepeatValues[2] == 1):
                if abs(sortedRepeatItems[0][0] - sortedRepeatItems[1][0]) == 1:
                    self._type = PokerType.plane
                else:
                    self._type = PokerType.noneType
            elif sortedRepeatValues[0] == 2 and sortedRepeatValues[1] == 2 and \
                    sortedRepeatValues[2] == 2 and sortedRepeatValues[3] == 2:
                if self._isContinuous():
                    self._type = PokerType.continuousDouble
                else:
                    self._type = PokerType.noneType
            elif sortedRepeatValues[0] == 1:
                if self._isContinuous():
                    self._type = PokerType.continuousSingle
                else:
                    self._type = PokerType.noneType
            else:
                self._type = PokerType.noneType

    def sortPokers(self) -> None:
        self._pokers.sort(reverse=False)
        self._fetchFormatPokers()
        self._fetchRepeatDict()

    def _fetchFormatPokers(self) -> None:
        self._formatPokers = self._pokers.copy()
        for idx, poker in enumerate(self._pokers):
            self._formatPokers[idx] = poker.getValueNoColor()

    def _fetchRepeatDict(self) -> None:
        self._repeatDict = {}
        for poker in self._formatPokers:
            self._repeatDict[poker] = self._repeatDict.get(poker, 0) + 1

    def _isContinuous(self) -> bool:
        keys = sorted(self._repeatDict.keys())
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
        self.sortAndFetchType()
        other.sortAndFetchType()
        leftType = self._type
        rightType = other._type

        if rightType == PokerType.empty:
            if leftType == PokerType.noneType:
                return False
            else:
                return True
        elif rightType == PokerType.single:
            if leftType == PokerType.single:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.double:
            if leftType == PokerType.double:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.triple:
            if leftType == PokerType.triple:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.tripleOne:
            if leftType == PokerType.tripleOne:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.tripleTwo:
            if leftType == PokerType.tripleTwo:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.quadrupleDouble:
            if leftType == PokerType.quadrupleDouble:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.continuousSingle:
            if leftType == PokerType.continuousSingle:
                if len(self._formatPokers) == len(other._formatPokers):
                    return self._compareFirstMostValue(other)
                else:
                    return False
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.continuousDouble:
            if leftType == PokerType.continuousDouble:
                if len(self._formatPokers) == len(other._formatPokers):
                    return self._compareFirstMostValue(other)
                else:
                    return False
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.plane:
            if leftType == PokerType.plane:
                if len(self._formatPokers) == len(other._formatPokers):
                    return self._compareFirstMostValue(other)
                else:
                    return False
            elif leftType == PokerType.singleBoom or leftType == PokerType.jokerBoom \
                    or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.singleBoom:
            if leftType == PokerType.singleBoom:
                return self._compareFirstMostValue(other)
            elif leftType == PokerType.jokerBoom or leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.jokerBoom:
            if leftType == PokerType.doubleBoom:
                return True
            else:
                return False
        elif rightType == PokerType.doubleBoom:
            if leftType == PokerType.doubleBoom:
                return self._compareFirstMostValue(other)
            else:
                return False

    def _compareFirstMostValue(self, other) -> bool:
        leftDict = self._repeatDict
        rightDict = other._repeatDict

        leftValue = leftDict.values()
        leftItem = leftDict.items()
        rightItem = rightDict.items()

        mostCount = max(leftValue)
        leftMostCountKey = [item[0] for item in leftItem if item[1] == mostCount]
        rightMostCountKey = [item[0] for item in rightItem if item[1] == mostCount]

        leftMostCountKey.sort()
        rightMostCountKey.sort()

        return leftMostCountKey[0] > rightMostCountKey[0]


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
    pokerStack = PokerStack()

    playerA = SimplePlayer(0, pokerStack)
    playerB = SimplePlayer(1, pokerStack)
    playerC = SimplePlayer(2, pokerStack)
    landlordThree = SimpleLandlordPokers(pokerStack)

    print(playerA)
    print(playerB)
    print(playerC)
    print(landlordThree)

    print(playerA.showPokers())
    index = list(map(int, input('出牌：\r\n').split()))
    print(playerA.outPokers(index, Pokers()))
    print(playerA.showPokers())


if __name__ == '__main__':
    main()
