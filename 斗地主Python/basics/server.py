#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/4 14:52
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : server.py
# @Software: PyCharm
from flask import Flask, request, jsonify
from game import Game, Pokers, PokerType
from basic import ReturnState


class Server(Game):
    def __init__(self):
        super().__init__()

        self.__ipList = []
        self.__currentId = -1

    def getIPList(self):
        return self.__ipList

    def setIPList(self, newIpList):
        self.__ipList = newIpList

    def getCurrentId(self):
        return self.__currentId

    def setCurrentId(self, id_):
        self.__currentId = id_


app = Flask(__name__)
server = Server()
g_previous = Pokers()
g_skipCnt = 0


@app.route('/')
def index():
    return "Index Page"


@app.route('/player', methods=['GET', 'POST'])
def player():
    if request.method == 'GET':
        return jsonify("Get Player")
    elif request.method == 'POST':
        post = request.form
        currentList = server.getIPList()
        currentList.append(post['ip'])
        server.setIPList(currentList)
        server.appendPlayer()
        num = server.getPlayerNum()
        if num == 3:
            server.setLandlord(server.getPlayerList()[0])
            server.setCurrentId(server.getLandlordId())

        return jsonify(
            {
                'player_id': num - 1
            }
        )


@app.route('/pokers/<int:playerId>')
def getPokers(playerId):
    rlt = server.getPlayerList()[playerId].showPokers()
    return jsonify(rlt)


@app.route('/pokers', methods=['POST'])
def postPokers():
    global g_previous, g_skipCnt

    if g_skipCnt == 2:
        g_skipCnt = 0
        g_previous = Pokers()

    post = request.form
    id_ = int(post['player_id'])
    cmd = post['cmd']
    if id_ != server.getCurrentId():
        return jsonify(ReturnState.notCurrent.value)

    currentPlayer = server.getPlayerList()[id_]
    cmdList = list(map(int, cmd.split()))
    if len(cmdList) == 0:
        return jsonify(ReturnState.errorOut.value)
    elif cmdList[0] == -1:
        g_previous.sortAndFetchType()
        if g_previous.getType() == PokerType.empty:
            return jsonify(ReturnState.cannotSkip.value)
        else:
            g_skipCnt += 1
            server.setCurrentId((server.getCurrentId() + 1) % 3)
            return jsonify(ReturnState.canSkip.value)
    else:
        out = currentPlayer.outPokers(cmdList, g_previous)
        if out:
            g_previous = out
            server.setCurrentId((server.getCurrentId() + 1) % 3)
            g_skipCnt = 0
            return jsonify(ReturnState.succeedOut.value)
        else:
            return jsonify(ReturnState.errorOut.value)


@app.route('/previous')
def getPrevious():
    global g_previous
    return jsonify(str(g_previous))


@app.route('/current')
def current():
    return jsonify(server.getCurrentId())


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
    # app.run(port=5000, debug=True)
