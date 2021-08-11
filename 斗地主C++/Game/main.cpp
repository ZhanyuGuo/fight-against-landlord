#include <iostream>
#include <memory>
#include "poker.h"
#include "game.h"
#include "onlinegame.h"

void localTest()
{
	using std::string;
	//using Tester = PokerGame::FAL::StupidLocalPlayerForDebugging;
	using Tester = PokerGame::FAL::ManualLocalPlayer;
	std::shared_ptr<PokerGame::FAL::Player>
		p1(new Tester(string("Tom"))),
		p2(new Tester(string("Foo"))),
		p3(new Tester(string("Zhw")));

	PokerGame::FAL::GameProcess game(p1, p2, p3);

	game.Run();

	int a;
	std::cin >> a;
}

void onlineTest()
{
	using namespace std::string_literals;

	WORD wsaVR = MAKEWORD(2, 2);
	WSADATA lp;
	int startSucc = WSAStartup(wsaVR, &lp);
	if (startSucc != 0)
	{
		return;
	}

	PokerGame::FAL::OnlineServer server("ServerConfig.json"s);
	server.Init();
	server.Start();

	int a;
	std::cin >> a;
}

int main()
{
	onlineTest();
	return 0;
}

