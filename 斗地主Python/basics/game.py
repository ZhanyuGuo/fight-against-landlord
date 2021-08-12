#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/1 14:57
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : game.py
# @Software: PyCharm
from basic import Pokers, PokerStack, SimplePlayer, SimpleLandlordPokers, PokerType


class Game(object):
    def __init__(self):
        self._pokerStack = PokerStack()
        self._playerNum = 0
        self._playerList = []
        self._landlordPokers = SimpleLandlordPokers(self._pokerStack)
        self._landlordId = -1

    def getPlayerList(self) -> list:
        return self._playerList

    def getLandlordId(self) -> int:
        return self._landlordId

    def getPlayerNum(self) -> int:
        return self._playerNum

    def appendPlayer(self) -> SimplePlayer:
        self._playerNum += 1
        newPlayer = SimplePlayer(self._playerNum - 1, self._pokerStack)
        self._playerList.append(newPlayer)
        return newPlayer

    def setLandlord(self, player: SimplePlayer) -> None:
        player.extendPokers(self._landlordPokers.getPokers())
        player.sortPokers()
        self._landlordId = player.getId()

    def decideLandLord(self, beginnerId: int) -> bool:
        score = [0] * 3
        print('================ 抢地主环节 ================')
        for _ in range(3):
            maxScore = max(score)
            if maxScore == 0:
                print(f"玩家{beginnerId}: ", end='')
                score[beginnerId] = int(input('输入分数(0, 1, 2, 3): '))
            elif maxScore == 1:
                print(f"玩家{beginnerId}: ", end='')
                score[beginnerId] = int(input('输入分数(0, 2, 3): '))
            elif maxScore == 2:
                print(f"玩家{beginnerId}: ", end='')
                score[beginnerId] = int(input('输入分数(0, 3): '))
            elif maxScore == 3:
                break
            else:
                break
            beginnerId = (beginnerId + 1) % 3

        maxScore = max(score)
        if maxScore == 0:
            print('================ 重新发牌 =================')
            return False
        else:
            maxScoreId = score.index(maxScore)
            self.setLandlord(self._playerList[maxScoreId])
            print(f"玩家{maxScoreId}以{maxScore}分，成为地主！")
            print('================ 抢地主结束 ================')
            return True

    def runAGame(self) -> None:
        player0 = self._playerList[0]
        player1 = self._playerList[1]
        player2 = self._playerList[2]

        currentId = self._landlordId
        previous = Pokers()
        skipCnt = 0
        while True:
            print('========================================')
            length0 = len(player0.getPokers())
            length1 = len(player1.getPokers())
            length2 = len(player2.getPokers())
            if not length0 or not length1 or not length2:
                break

            if skipCnt == 2:
                skipCnt = 0
                previous = Pokers()

            currentPlayer = self._playerList[currentId]
            print('上家：', end='')
            print(previous)
            print(f"玩家{currentId}: ", end='')
            print(currentPlayer.showPokers())
            cmd = list(map(int, input('选择牌号（空格间隔），或输入-1跳过:\r\n').split()))
            if len(cmd) == 0:
                continue
            elif cmd[0] == -1:
                previous.sortAndFetchType()
                if previous.getType() == PokerType.empty:
                    print('不可跳过')
                    continue
                else:
                    print(f"玩家{currentId}跳过...")
                    currentId = (currentId + 1) % 3
                    skipCnt += 1
                    continue
            else:
                out = currentPlayer.outPokers(cmd, previous)
                if out:
                    previous = out
                    currentId = (currentId + 1) % 3
                    skipCnt = 0
                    continue
                else:
                    print('出牌错误，请重新选择！')
                    continue

        if length0 == 0:
            if self._landlordId == 0:
                print('地主获胜')
            else:
                print('农民获胜')
        elif length1 == 0:
            if self._landlordId == 1:
                print('地主获胜')
            else:
                print('农民获胜')
        else:
            if self._landlordId == 2:
                print('地主获胜')
            else:
                print('农民获胜')

    def run(self) -> None:
        while True:
            self.__init__()
            playerA = self.appendPlayer()
            playerB = self.appendPlayer()
            playerC = self.appendPlayer()
            print(playerA)
            print(playerB)
            print(playerC)
            if self.decideLandLord(0):
                self.runAGame()
            else:
                continue
            if input('开始下一局？y/[n]') != 'y':
                break


def main():
    game = Game()
    game.run()


if __name__ == '__main__':
    main()
