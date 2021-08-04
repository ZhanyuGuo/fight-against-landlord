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


class Client(object):
    def __init__(self, ip, id_, port=5000):
        self.url = f"http://{ip}:{port}/"
        self.id = id_
        pass

    @staticmethod
    def getLocalIP():
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            s.connect(('8.8.8.8', 80))
            ip = s.getsockname()[0]
        finally:
            s.close()
        return ip

    def postPlayer(self):
        post = {'ip': self.getLocalIP()}
        r = requests.post(self.url + "player", data=post)
        rlt = r.json()
        return rlt

    def getPokersFromServer(self):
        r = requests.get(self.url + f"pokers/{self.id}")
        rlt = r.json()
        return str(rlt)

    def postPokersToServer(self, index):
        post = {'id': self.id, 'idx': index}
        r = requests.post(self.url + "pokers", data=post)
        rlt = r.json()
        return rlt

    def getCurrentPlayer(self):
        r = requests.get(self.url + "current")
        rlt = r.json()
        return int(rlt)

    pass


def getLogTime():
    return f"[{strftime('%H:%M:%S')}]"


def main():
    print(f"{getLogTime()} 欢迎！斗地主v1.0！")
    ip_address = input(f"{getLogTime()} 请输入服务器IP地址：[127.0.0.1]\r\n")
    if ip_address == '':
        ip_address = "127.0.0.1"

    player_id = int(input(f"{getLogTime()} 请输入玩家ID号(0, 1, 2)：\r\n"))
    client = Client(ip_address, player_id)
    rlt = client.postPlayer()
    input()
    rlt = client.postPokersToServer('')
    print(rlt)
    # while True:
    #     input(f"{getLogTime()} 输入回车刷新：")
    #     rlt = client.getPokersFromServer()
    #     print(rlt)
    #     cur = client.getCurrentPlayer()
    #     if cur != client.id:
    #         continue

    pass


if __name__ == '__main__':
    main()
