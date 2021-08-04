#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Time    : 2021/8/4 14:52
# @Author  : Zhanyu Guo
# @Email   : 942568052@qq.com
# @File    : server.py
# @Software: PyCharm
from flask import Flask, request, jsonify
from game import Game


class Server(Game):
    def __init__(self):
        super().__init__()

        self._ip_list = []
        self.current_id = -1

    def getIPList(self):
        return self._ip_list

    def setIPList(self, new_ip_list):
        self._ip_list = new_ip_list

    pass


app = Flask(__name__)
server = Server()


@app.route('/')
def index():
    return "Index Page"


@app.route('/player', methods=['GET', 'POST'])
def player():
    if request.method == 'GET':

        return jsonify("Get Player")
    elif request.method == 'POST':
        cur_list = server.getIPList()
        cur_list.append(request.form['ip'])
        server.setIPList(cur_list)
        server.addPlayer()
        if len(cur_list) == 3:
            server.setLandlord(server.player_list[0])
            server.current_id = server.landlord_id
            pass

        return jsonify("Post Player")

    pass


@app.route('/pokers/<int:player_id>')
def getPokers(player_id):
    rlt = server.player_list[player_id].showPokers()
    return jsonify(rlt)


@app.route('/pokers', methods=['POST'])
def postPokers():
    post = request.form
    id_ = post['id']
    index = post['idx']
    return jsonify(id_)


@app.route('/current')
def current():
    return jsonify(server.current_id)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
    # app.run(port=5000, debug=True)
