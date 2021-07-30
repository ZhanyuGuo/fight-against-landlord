#include <iostream>
#include <memory>
#include "poker.h"
#include "game.h"

using std::string;

void test() noexcept
{
	throw std::exception();
}


int main()
{
	//system("chcp 65001");
	
	using Tester = PokerGame::FAL::StupidLocalPlayerForDebugging;
	std::shared_ptr<PokerGame::FAL::Player>
		p1(new Tester(string("Tom"))),
		p2(new Tester(string("Foo"))),
		p3(new Tester(string("Zhw")));

	PokerGame::FAL::GameProcess game(p1, p2, p3);

	game.Run();

	int a;
	std::cin >> a;
	return 0;
}

