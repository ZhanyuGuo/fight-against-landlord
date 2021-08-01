#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/1 14:57
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : game.py
# @Software: PyCharm
from base import *


class Game(object):
    def __init__(self):
        self.poker_stack = PokerStack()
        self.player_num = 0
        self.player_list = []
        self.landlord_pokers = SimpleLandlordPoker(self.poker_stack)
        self.landlord_id = -1

    def addPlayer(self):
        self.player_num += 1
        new_player = SimplePlayer(self.player_num - 1, self.poker_stack)
        self.player_list.append(new_player)
        return new_player

    def setLandlord(self, player: SimplePlayer):
        player.pokers.extend(self.landlord_pokers.pokers)
        player.sortPokers()
        self.landlord_id = player.id

    def decideLandLord(self, beginner_id):
        score = [0] * 3
        print('================ 抢地主环节 ================')
        for _ in range(3):
            max_score = max(score)
            if max_score == 0:
                print(f"玩家{beginner_id}: ", end='')
                score[beginner_id] = int(input('输入分数(0, 1, 2, 3): '))
            elif max_score == 1:
                print(f"玩家{beginner_id}: ", end='')
                score[beginner_id] = int(input('输入分数(0, 2, 3): '))
            elif max_score == 2:
                print(f"玩家{beginner_id}: ", end='')
                score[beginner_id] = int(input('输入分数(0, 3): '))
            elif max_score == 3:
                break
            else:
                break
            beginner_id = (beginner_id + 1) % 3
            pass

        max_score = max(score)
        if max_score == 0:
            print('================ 重新发牌 =================')
            return False
        else:
            max_score_id = score.index(max_score)
            self.setLandlord(self.player_list[max_score_id])
            print(f"玩家{max_score_id}以{max_score}分，成为地主！")
            print('================ 抢地主结束 ================')
            return True

    def run_a_game(self):
        player_0 = self.player_list[0]
        player_1 = self.player_list[1]
        player_2 = self.player_list[2]

        current_id = self.landlord_id
        previous = Pokers()
        skip_cnt = 0
        while True:
            print('========================================')
            length_0 = len(player_0.pokers)
            length_1 = len(player_1.pokers)
            length_2 = len(player_2.pokers)
            if not length_0 or not length_1 or not length_2:
                break

            if skip_cnt == 2:
                skip_cnt = 0
                previous = Pokers()

            current_player = self.player_list[current_id]
            print('上家：', end='')
            print(previous)
            print(f"玩家{current_id}: ", end='')
            current_player.showPokers()
            cmd = list(map(int, input('选择牌号（空格间隔），或输入-1跳过:\r\n').split()))
            if len(cmd) == 0:
                continue
            elif cmd[0] == -1:
                if previous.getType() == PokerType.empty:
                    print('不可跳过')
                    continue
                else:
                    print(f"玩家{current_id}跳过...")
                    current_id = (current_id + 1) % 3
                    skip_cnt += 1
                    continue
            else:
                out = current_player.outPoker(cmd, previous)
                if out:
                    previous = out
                    current_id = (current_id + 1) % 3
                    skip_cnt = 0
                    continue
                else:
                    print('出牌错误，请重新选择！')
                    continue

        if length_0 == 0:
            if self.landlord_id == 0:
                print('地主获胜')
            else:
                print('农民获胜')
        elif length_1 == 0:
            if self.landlord_id == 1:
                print('地主获胜')
            else:
                print('农民获胜')
        else:
            if self.landlord_id == 2:
                print('地主获胜')
            else:
                print('农民获胜')

    def run(self):
        while True:
            self.__init__()
            player_a = self.addPlayer()
            player_b = self.addPlayer()
            player_c = self.addPlayer()
            print(player_a)
            print(player_b)
            print(player_c)
            if self.decideLandLord(0):
                self.run_a_game()
            else:
                continue
            if input('开始下一局？y/[n]') != 'y':
                break


def main():
    game = Game()
    game.run()


if __name__ == '__main__':
    main()
