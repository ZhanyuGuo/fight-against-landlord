#include "onlinegame.h"
#include <thread>

namespace PokerGame
{
	namespace FAL
	{

		void OnlineServer::Startup()
		{
			auto broadcastSceneThread = std::thread(&OnlineServer::BroadCastSceneThread, this);
			broadcastSceneThread.detach();
		}

		void OnlineServer::Reset()
		{
			this->resetFlag = 0b1;
			while (this->resetFlag)
			{

			}
		}

		void OnlineServer::Init()
		{
			try
			{
				this->NetInit();
				this->GameReset();
			}
			catch (ServerInitFailedException&)
			{
				throw;
			}
		}

		OnlineServer::OnlineServer()
		{
			this->port = 6666;
			
		}

		OnlineServer::OnlineServer(int port)
		{
			throw std::exception("not implemented yet");
		}

		void OnlineServer::BroadCastSceneThread()
		{
			while (true)
			{
				int reset = this->resetFlag & 0b1;
				if (reset)
				{
					this->resetFlag &= 0b0;
					return;
				}

				Scene current = this->FormCurrentScene();
				int sentlen = sendto(this->broadcastSocket_fd,
					reinterpret_cast<char*>(&current),
					sizeof(Scene),
					0,
					reinterpret_cast<sockaddr*>(&this->clientBroadCastAddr),
					sizeof(sockaddr_in));
				using namespace std::chrono_literals;
				if (sentlen == -1)
				{

				}
				std::this_thread::sleep_for(0.5s);
			}
		}

		void OnlineServer::ListenThread()
		{
		}

		void OnlineServer::NetInit()
		{
			this->broadcastSocket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
			if (this->broadcastSocket_fd == INVALID_SOCKET)
			{
				throw ServerInitFailedException();
			}
			this->clientBroadCastAddr.sin_family = AF_INET;
			this->clientBroadCastAddr.sin_port = htons(this->port);
			in_addr broadcastAddr;
			inet_pton(AF_INET, "224.0.1.0", &broadcastAddr.s_addr);
			inet_pton(AF_INET, "224.0.1.0", &this->clientBroadCastAddr.sin_addr.s_addr);
			int setMultiCastResult = setsockopt(this->broadcastSocket_fd, IPPROTO_IP, IP_MULTICAST_IF, reinterpret_cast<char*>(&broadcastAddr), sizeof(in_addr));
			if (setMultiCastResult != 0)
			{
				throw ServerInitFailedException();
			}
		}

		void OnlineServer::GameReset()
		{
			this->gameStage = STAGE_PREPARING;
			ClientID playerIDs[3];
			this->isLandlordDetermined = false;
			std::fill(std::begin(this->landlordWillingness), std::end(this->landlordWillingness), '\0');
			this->landlordIndex = 0;
			this->nextActPlayerIndex = -1;
			this->nextActType = ACTIVE_TYPE_ACTIVE;
			this->lastActType = ACTIVE_TYPE_PASS;
			this->secondLastActType = ACTIVE_TYPE_PASS;
			this->winnerFlag = 0b000;
			this->lastAct.reset(new IdBasedCardCollection());
			this->secondLastAct.reset(new IdBasedCardCollection());
			for (int i = 0; i < 3; i++)
			{
				this->playerCards[i].reset(new IdBasedCardCollection());
			}
			this->lordCards.reset(new IdBasedCardCollection());
		}

		Scene OnlineServer::FormCurrentScene() noexcept
		{
			Scene currentScene;
			currentScene.CurrentStage = static_cast<char>(this->gameStage);
			this->lastAct->Serialize(currentScene.LastCardDrop, 20);
			this->secondLastAct->Serialize(currentScene.SecondLastCardDrop, 20);
			currentScene.ActiveIndex = static_cast<char>(this->nextActPlayerIndex);
			currentScene.ActiveType = this->nextActType;
			for (int i = 0; i < 3; i++)
			{
				currentScene.CardLeftCount[i] = static_cast<char>(this->playerCards[i]->Count());
			}
			this->lordCards->Serialize(currentScene.LandlordCards, 3);
			if (this->isLandlordDetermined)
			{
				currentScene.LandlordIndex = static_cast<char>(this->landlordIndex);
			}
			else
			{
				currentScene.LandlordIndex = static_cast<char>(-1);
			}
			std::copy(std::begin(this->landlordWillingness), 
				std::end(this->landlordWillingness), 
				std::begin(currentScene.LandlordWillingness));
			currentScene.LastActType = this->lastActType;
			currentScene.SecondLastActType = this->secondLastActType;
			currentScene.WinnerFlag = static_cast<char>(this->winnerFlag);
			return currentScene;
		}
	}
}