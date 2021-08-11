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

		// 以名称的hash code和ip地址作为客户端的身份标识
		struct ClientID
		{
		public:
			int nameHashCode;
			sockaddr_in ip;
			bool operator== (ClientID& other)
			{
				if (this->nameHashCode != other.nameHashCode)
				{
					return false;
				}
				long long thisAddr = *reinterpret_cast<long long*>(&(this->ip));
				long long otherAddr = *reinterpret_cast<long long*>(&(other.ip));
				return (this->nameHashCode == other.nameHashCode);
			}
		};

		// 1478206855 是C#中"NULL".GetHashCode()的返回值
		constexpr ClientID NullPlayer = { 1478206855 , {AF_INET, INADDR_ANY, 0} };

		class OnlineServer
		{
		public:
			void Start();
			void Reset();
			/// <summary>
			/// 初始化此Server, 调用Start()方法前务必调用此方法, 否则行为是未定义的
			/// </summary>
			void Init();
			/// <summary>
			/// 使用默认的值进行构造, 通常不可移植, 需要手动将源文件中的值设置为本机值然后重新生成
			/// </summary>
			OnlineServer();
			/// <summary>
			/// 从Json文件加载参数
			/// </summary>
			/// <param name="configPath">Json文件的路径</param>
			OnlineServer(std::string configPath);
		private:
			void BroadCastSceneThread();
			void ListenThread();
			void NetInit();
			void GameReset();
			Scene FormCurrentScene() noexcept;
		private:
			int multicastPort;
			int multicastLocalBindPort;
			int serviceListenPort;
			std::string multicastIP;
			std::string multicastLocalBindIP;
			sockaddr_in clientBroadCastAddr;
			SOCKET broadcastSocketFD;
			SOCKET serviceSocketFD;
			volatile ClientID playerIDs[3];
		private:
			volatile int resetFlag;
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
