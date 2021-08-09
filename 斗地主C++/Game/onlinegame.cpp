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
			this->broadcastSocket_fd = socket(AF_INET, SOCK_DGRAM, 0);
			if (this->broadcastSocket_fd == INVALID_SOCKET)
			{
				throw ServerInitFailedException();
			}
			this->clientBroadCastAddr.sin_family = AF_INET;
			this->clientBroadCastAddr.sin_port = this->port;
			in_addr broadcastAddr;
			inet_pton(AF_INET, "239.0.1.10", &broadcastAddr.s_addr);
			inet_pton(AF_INET, "239.0.1.10", &this->clientBroadCastAddr.sin_addr.s_addr);
			int setMultiCastResult = setsockopt(this->broadcastSocket_fd, IPPROTO_IP, IP_MULTICAST_IF, reinterpret_cast<char*>(&broadcastAddr), sizeof(in_addr));
			if (setMultiCastResult != 0)
			{
				throw ServerInitFailedException();
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

		Scene OnlineServer::FormCurrentScene() noexcept
		{
			Scene currentScene;
			currentScene.CurrentStage = static_cast<char>(this->gameStage);
			


			return currentScene;
		}
	}
}