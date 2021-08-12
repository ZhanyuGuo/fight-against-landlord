#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/4 14:52
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : client.py
# @Software: PyCharm
from time import strftime
import socket
import requests
from basic import ReturnState


class Client(object):
    def __init__(self, ip, port=5000):
        self.__url = f"http://{ip}:{port}/"
        self.__id = -1

    def getId(self):
        return self.__id

    @staticmethod
    def getLocalIP():
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            s.connect(('8.8.8.8', 80))
            ip = s.getsockname()[0]
        finally:
            s.close()
        return ip

    def postPlayerToServer(self):
        post = {'ip': self.getLocalIP()}
        r = requests.post(self.__url + "player", data=post)
        rlt = r.json()
        self.__id = int(rlt['player_id'])
        return rlt

    def getCurrentPlayerFromServer(self):
        r = requests.get(self.__url + "current")
        rlt = r.json()
        return int(rlt)

    def getPokersFromServer(self):
        r = requests.get(self.__url + f"pokers/{self.__id}")
        rlt = r.json()
        return str(rlt)

    def getPreviousFromServer(self):
        r = requests.get(self.__url + "previous")
        rlt = r.json()
        return rlt

    def postCmdToServer(self, cmd_):
        post = {'player_id': self.__id, 'cmd': cmd_}
        r = requests.post(self.__url + "pokers", data=post)
        rlt = r.json()
        return int(rlt)

    # def __del__(self):
    #     pass


def getLogTime():
    return f"[{strftime('%H:%M:%S')}] "


def main():
    print(f"{getLogTime()}欢迎！斗地主v1.0！[默认输入]")
    ipAddress = input(f"{getLogTime()}请输入服务器IP地址：[127.0.0.1] ")
    if ipAddress == '':
        ipAddress = "127.0.0.1"

    client = Client(ipAddress)
    rlt = client.postPlayerToServer()
    print(getLogTime(), rlt)

    while True:
        input(f"{getLogTime()}等待玩家，输入回车刷新：")
        rlt = client.getCurrentPlayerFromServer()
        if rlt != -1:
            break

    while True:
        input(f"{getLogTime()}等待出牌，输入回车刷新：")
        rlt = client.getPokersFromServer()
        print(f"{getLogTime()}你是玩家{client.getId()}，你的手牌是：")
        print(rlt)
        currentPlayer = client.getCurrentPlayerFromServer()
        previous = client.getPreviousFromServer()
        print(f"{getLogTime()}当前出牌：玩家{currentPlayer}，最新出牌：{previous}")
        if currentPlayer == client.getId():
            while True:
                cmd = input(f"{getLogTime()}选择牌号（空格间隔），或输入-1跳过：")
                rlt = client.postCmdToServer(cmd)
                if rlt == ReturnState.succeedOut:
                    print(f"{getLogTime()}出牌成功")
                    break
                elif rlt == ReturnState.errorOut:
                    print(f"{getLogTime()}出牌错误")
                    continue
                elif rlt == ReturnState.canSkip:
                    print(f"{getLogTime()}跳过成功")
                    break
                elif rlt == ReturnState.cannotSkip:
                    print(f"{getLogTime()}不可跳过")
                elif rlt == ReturnState.notCurrent:
                    break
                else:
                    break


if __name__ == '__main__':
    main()
