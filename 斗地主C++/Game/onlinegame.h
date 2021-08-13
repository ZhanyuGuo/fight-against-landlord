#pragma once
#ifdef _WIN32
	#include <WinSock2.h>
	#include <ws2tcpip.h> 
	#pragma comment(lib, "ws2_32.lib")
#endif // WIN32

#include <string>
#include <memory>
#include <mutex>
#include <random>
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
		constexpr auto ACTIVE_PARAM_1 = ((char)1);
		constexpr auto ACTIVE_PARAM_2 = ((char)2);
		constexpr auto ACTIVE_PARAM_3 = ((char)3);
		constexpr int CLIENT_DATAGRAM_TYPE_JOIN = 1;
		constexpr int CLIENT_DATAGRAM_TYPE_READY = 2;
		constexpr int CLIENT_DATAGRAM_TYPE_GETCARD = 3;
		constexpr int CLIENT_DATAGRAM_TYPE_POSTPOINT = 4;
		constexpr int CLIENT_DATAGRAM_TYPE_POSTCARD = 5;

		struct Scene
		{
			char CurrentStage;
			char LandlordWillingness[3];
			char LandlordIndex;
			char ActiveIndex;
			char ActiveParam;
			char CardLeftCount[3];
			char LandlordCards[3];
			char LastActType;
			char SecondLastActType;
			char LastCardDrop[20];
			char SecondLastCardDrop[20];
			char WinnerFlag;
			char ReadyFlag;
			unsigned char ServiceAddr[4];
		};

		// 以名称的hash code和ip地址作为客户端的身份标识
		struct ClientID
		{
		public:
			int nameHashCode;
			sockaddr_in ip;
			bool operator== (const ClientID& other) const
			{
				if (this->nameHashCode != other.nameHashCode)
				{
					return false;
				}
				// 只比较ip是否相同，不管端口号
				auto thisIP = this->ip.sin_addr.s_addr;
				auto otherIP = other.ip.sin_addr.s_addr;
				return (thisIP == otherIP);
			}
			bool operator!= (const ClientID& other) const
			{
				return !(*this == other);
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
			void ClientIDReset();
			void PlayerCardsReset();
			void HandleMessage(char* buf, int bufLen, sockaddr_in addr);
			void WriteBack(sockaddr_in addr, std::string content);
			void WriteBack(sockaddr_in addr, char* buf, int len);
			void HandlePlayerPrepare(int playerIndex);
			void HandlePlayerDeterminLandlord(int playerIndex, int willingNess);
			void HandlePlayerCardAct(int playerIndex, char* cardAct, int len);
			void HandoutInitialCards();
			int FindPlayerIndex(int nameHash, sockaddr_in addr) noexcept;
			Scene FormCurrentScene() noexcept;
			inline int GetAvailableIndex();
			inline int IsAllPrepared();
			inline void SetLandlord(int playerIndex);
			inline void MoveActiveIndexToNext();
		private://网络通信相关字段
			int multicastPort;
			int multicastLocalBindPort;
			int serviceListenPort;
			std::string multicastIP;
			std::string multicastLocalBindIP;
			sockaddr_in clientBroadCastAddr;
			sockaddr_in serviceAddr;
			SOCKET broadcastSocketFD;
			SOCKET serviceSocketFD;
			ClientID playerIDs[3];
		private://游戏数据相关字段
			volatile int resetFlag;
			char gameStage;
			int startIndex;
			std::default_random_engine rng;
			bool isLandlordDetermined;
			char landlordWillingness[3];
			int landlordIndex;
			int nextActPlayerIndex;
			char nextActParam;
			char lastActType;
			char secondLastActType;
			int winnerFlag;
			int playerReadyFlag;
			std::unique_ptr<PokerCardCollection> lastAct;
			std::unique_ptr<PokerCardCollection> secondLastAct;
			std::unique_ptr<PokerCardCollection> playerCards[3];
			std::unique_ptr<PokerCardCollection> lordCards;
		private://线程同步
			std::mutex handlePrepareMutex;
			std::mutex handleCardMutex;
			std::mutex handleDetermineLandlordMutex;
		};

		class ServerInitFailedException : public std::exception
		{

		};


	}
}
