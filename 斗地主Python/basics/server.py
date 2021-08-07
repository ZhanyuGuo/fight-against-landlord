#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/4 14:52
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : server.py
# @Software: PyCharm
from flask import Flask, request, jsonify
from game import Game, Pokers, PokerType
from net_basic import ReturnState


class Server(Game):
    def __init__(self):
        super().__init__()

        self.__ip_list = []
        self.__current_id = -1

    def getIPList(self):
        return self.__ip_list

    def setIPList(self, new_ip_list):
        self.__ip_list = new_ip_list

    def getCurrentId(self):
        return self.__current_id

    def setCurrentId(self, id_):
        self.__current_id = id_

    pass


app = Flask(__name__)
server = Server()
g_previous = Pokers()
g_skip_cnt = 0


@app.route('/')
def index():
    return "Index Page"


@app.route('/player', methods=['GET', 'POST'])
def player():
    if request.method == 'GET':
        return jsonify("Get Player")
    elif request.method == 'POST':
        post = request.form
        current_list = server.getIPList()
        current_list.append(post['ip'])
        server.setIPList(current_list)
        server.addPlayer()
        num = server.getPlayerNum()
        if num == 3:
            server.setLandlord(server.getPlayerList()[0])
            server.setCurrentId(server.getLandlordId())

        return jsonify(
            {
                'player_id': num - 1
            }
        )


@app.route('/pokers/<int:player_id>')
def getPokers(player_id):
    rlt = server.getPlayerList()[player_id].showPokers()
    return jsonify(rlt)


@app.route('/pokers', methods=['POST'])
def postPokers():
    global g_previous, g_skip_cnt

    if g_skip_cnt == 2:
        g_skip_cnt = 0
        g_previous = Pokers()

    post = request.form
    id_ = int(post['player_id'])
    cmd = post['cmd']
    if id_ != server.getCurrentId():
        return jsonify(ReturnState.not_current.value)

    current_player = server.getPlayerList()[id_]
    cmd_list = list(map(int, cmd.split()))
    if len(cmd_list) == 0:
        return jsonify(ReturnState.error_out.value)
    elif cmd_list[0] == -1:
        g_previous.sortGetType()
        if g_previous.getType() == PokerType.empty:
            return jsonify(ReturnState.cannot_skip.value)
        else:
            g_skip_cnt += 1
            server.setCurrentId((server.getCurrentId() + 1) % 3)
            return jsonify(ReturnState.can_skip.value)
    else:
        out = current_player.outPokers(cmd_list, g_previous)
        if out:
            g_previous = out
            server.setCurrentId((server.getCurrentId() + 1) % 3)
            g_skip_cnt = 0
            return jsonify(ReturnState.succeed_out.value)
        else:
            return jsonify(ReturnState.error_out.value)


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
