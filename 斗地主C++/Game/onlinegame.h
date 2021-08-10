#pragma once
#ifdef _WIN32
	#include <WinSock2.h>
	#include <ws2tcpip.h> 
	#pragma comment(lib, "ws2_32.lib")
#endif // WIN32

#include <string>
#include <memory>

#include "poker.h"
#include "cardtype.h"

namespace PokerGame
{
	namespace FAL
	{
		constexpr auto STAGE_PREPARING = ((char)0);
		constexpr auto STAGE_DETERMINE_LANDLORD = ((char)1);
		constexpr auto STAGE_MAINLOOP_ONGOING = ((char)2);
		constexpr auto STAGE_END = ((char)3);
		constexpr auto ACTIVE_TYPE_ACTIVE = ((char)0);
		constexpr auto ACTIVE_TYPE_FOLLOW = ((char)1);
		constexpr auto ACTIVE_TYPE_PASS = ((char)2);

		struct Scene
		{
			char CurrentStage;
			char LandlordWillingness[3];
			char LandlordIndex;
			char ActiveIndex;
			char ActiveType;
			char CardLeftCount[3];
			char LandlordCards[3];
			char LastActType;
			char SecondLastActType;
			char LastCardDrop[20];
			char SecondLastCardDrop[20];
			char WinnerFlag;
		};

		//int scene_struct_len = sizeof(Scene);

		struct ClientID
		{
			int hashCode;
			sockaddr_in ip;
		};

		class OnlineServer
		{
		public:
			void Startup();
			void Reset();
			void Init();
			OnlineServer();
			OnlineServer(int port);
		private:
			void BroadCastSceneThread();
			void ListenThread();
			void NetInit();
			void GameReset();
			int port;
			volatile int resetFlag;
			sockaddr_in clientBroadCastAddr;
			SOCKET broadcastSocket_fd;
			Scene FormCurrentScene() noexcept;
			ClientID playerIDs[3];
			char gameStage;
			bool isLandlordDetermined;
			char landlordWillingness[3];
			int landlordIndex;
			int nextActPlayerIndex;
			char nextActType;
			char lastActType;
			char secondLastActType;
			int winnerFlag;
			std::unique_ptr<PokerCardCollection> lastAct;
			std::unique_ptr<PokerCardCollection> secondLastAct;
			std::unique_ptr<PokerCardCollection> playerCards[3];
			std::unique_ptr<PokerCardCollection> lordCards;
		};

		class ServerInitFailedException : public std::exception
		{

		};


	}
}
