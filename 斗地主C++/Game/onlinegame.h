#pragma once
#include <string>
#include <memory>
#include <WinSock2.h>
#include <ws2tcpip.h> 
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
		};

		int scene_struct_len = sizeof(Scene);

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
			int port;
			volatile int resetFlag;
			sockaddr_in clientBroadCastAddr;
			SOCKET broadcastSocket_fd;
			Scene FormCurrentScene() noexcept;
			ClientID playerIDs[3];
			int gameStage;
			int isLandlordDetermined;
			int landlordIndex;
			int nextActPlayerIndex;
			char nextActType;
			std::unique_ptr<PokerCardCollection> lastAct;
			std::unique_ptr<PokerCardCollection> secondLastAct;
			std::unique_ptr<PokerCardCollection> playerCards[3];

		};

		class ServerInitFailedException : public std::exception
		{

		};


	}
}
