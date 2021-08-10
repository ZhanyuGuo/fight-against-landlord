#include <iostream>
#include <memory>
#include "poker.h"
#include "game.h"
#include "onlinegame.h"

using std::string;

void test() noexcept
{
	throw std::exception();
}

class A {
public:
	A()
	{
		throw PokerGame::FAL::NotSameTypeException();
	}
private:
	char a[0x7fffffff];
};

void localTest()
{

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
	WORD wsaVR = MAKEWORD(2, 2);
	WSADATA lp;
	WSAStartup(wsaVR, &lp);

	PokerGame::FAL::OnlineServer server;
	server.Init();
	server.Startup();

	int a;
	std::cin >> a;
}

int main()
{
	onlineTest();


	
	return 0;
}

