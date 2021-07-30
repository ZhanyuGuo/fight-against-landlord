#include "game.h"
#include <memory>
#include <iostream>

namespace PokerGame
{
	namespace FAL
	{



		GameProcess::GameProcess(std::shared_ptr<Player>& p1,
			std::shared_ptr<Player>& p2,
			std::shared_ptr<Player>& p3)
		{
			this->players[0] = p1;
			this->players[1] = p2;
			this->players[2] = p3;
			this->Magnification = 1;
			this->LandlordIndex = 0;
			this->LandlordSelected = false;
			this->LordCards = std::unique_ptr<PokerCardCollection>(new IdBasedCardCollection());
		}

		GameProcess::~GameProcess()
		{

		}

		void GameProcess::Run()
		{
			// TODO: 考虑放弃各种循环，改用事件驱动的形式
			do
			{
				this->ResetGame();
				this->HandoutInitCards();
				this->DetermineLandlord();
			} while (!this->LandlordSelected);

			std::cout << "地主是" << this->LandlordIndex + 1 << "号" << std::endl;
			std::cout << "地主牌" << this->LordCards->ToString() << std::endl;

			for (int i = 0; i < 3; i++)
			{
				std::cout << this->players[i]->GetName() << "的手牌" << this->players[i]->GetCards().ToString() << std::endl;
			}

			this->MainCardLoop();

		}

		void GameProcess::ResetGame()
		{
			for (int i = 0; i < PlayerNum; i++)
			{
				this->players[i]->Reset();
			}
			this->Magnification = 1;
			this->LandlordIndex = 0;
			this->LandlordSelected = false;
			this->LordCards = std::unique_ptr<PokerCardCollection>(new IdBasedCardCollection());
		}

		void GameProcess::HandoutInitCards()
		{
			auto cardHeap = TakeOnlyCardCollection::Standard54();
			for (int i = 0; i < 51; i++)
			{
				int playerIndex = i % 3;
				PokerCardCollection& playerCard = this->players[playerIndex]->GetCards();
				cardHeap >> playerCard;
			}
			*(this->LordCards) << cardHeap;
		}

		void GameProcess::DetermineLandlord()
		{
			int startIndex = rand() % 3;
			int leastPoint = 0;
			for (int i = 0; i < PlayerNum; i++)
			{
				int playerIndex = (i + startIndex) % 3;
				int willingness = this->players[playerIndex]->PrepareResponse(leastPoint);
				if (willingness > leastPoint)
				{
					leastPoint = willingness;
					this->LandlordIndex = playerIndex;
					if (leastPoint == 3)
					{
						break;
					}
				}
			}
			if (leastPoint == 0)
			{
				this->LandlordSelected = false;
			}
			else
			{
				this->LandlordSelected = true;
				this->Magnification *= leastPoint;
				this->players[this->LandlordIndex]->GetCards() << *(this->LordCards);
			}
		}

		void GameProcess::MainCardLoop()
		{
			int idx = this->LandlordIndex;
			int passed = 0;
			using State = CardEvent::MotionType;
			State currentState = State::Active;

			std::shared_ptr<PokerCardCollection> currentCards(nullptr);
			while (true)
			{
				if (currentState == State::Active)
				{
					CardEvent activeMotionInfo(State::Active);
					auto activeResponse = this->players[idx]->CardResponse(activeMotionInfo);
					if (this->players[idx]->GetCards().Count() == 0)
					{
						break;
					}
					currentCards = activeResponse.GetContent();
					currentState = State::Follow;
					passed = 0;
				}
				else
				{
					CardEvent followMotionInfo(State::Follow, currentCards);
					auto followResponse = this->players[idx]->CardResponse(followMotionInfo);
					if (this->players[idx]->GetCards().Count() == 0)
					{
						std::cout << this->players[idx]->GetName() << "打完了" << std::endl;
						break;
					}
					switch (followResponse.GetType())
					{
					case State::Pass:
						passed += 1;
						if (passed == 2)
						{
							currentState = State::Active;
						}
						break;
					case State::Follow:
						currentCards = followResponse.GetContent();
						passed = 0;
						break;
					}

				}
				idx = (idx + 1) % 3;
			}
		}

		CardEvent::MotionType CardEvent::GetType() const
		{
			return this->type;
		}

		std::shared_ptr<PokerCardCollection> CardEvent::GetContent() const
		{
			return this->content;
		}

		CardEvent::CardEvent(MotionType type)
		{
			this->type = type;
			this->content = std::shared_ptr<PokerCardCollection>(nullptr);
		}

		CardEvent::CardEvent(MotionType type, std::shared_ptr<PokerCardCollection>& content)
		{
			this->type = type;
			this->content = content;
		}

		int StupidLocalPlayerForDebugging::PrepareResponse(int leastPoint) noexcept
		{
			return 3;
		}

		CardEvent StupidLocalPlayerForDebugging::CardResponse(CardEvent e) noexcept
		{
			int c = this->cards.Count();
			switch (e.GetType())
			{
			case CardEvent::MotionType::Active:
			{
				PokerCard card = this->cards.PickOut(c - 1);
				std::shared_ptr<PokerCardCollection> cl(new IdBasedCardCollection());
				*cl << card;
				std::cout << this->name << std::string(":打出了") << card.ToString(true) << std::endl;
				return CardEvent(CardEvent::MotionType::Follow, cl);
				break;
			}
			case CardEvent::MotionType::Follow:
			{
				PokerCard largestCard = this->cards[c - 1];
				if (largestCard.Get3BasedNum() > (*(e.GetContent()))[(*(e.GetContent())).Count() - 1].Get3BasedNum())
				{
					this->cards.PickOut(c - 1);
					std::shared_ptr<PokerCardCollection> cle(new IdBasedCardCollection());
					*cle << largestCard;
					std::cout << this->name << std::string(":跟随了") << largestCard.ToString(true) << std::endl;
					return CardEvent(CardEvent::MotionType::Follow, cle);
				}
				else
				{
					std::cout << this->name << std::string(":跳过") << std::endl;
					return CardEvent(CardEvent::MotionType::Pass);
				}
				break;
			}
			}
		}

		PokerCardCollection& StupidLocalPlayerForDebugging::GetCards()
		{
			return this->cards;
		}

		void StupidLocalPlayerForDebugging::Reset()
		{
			this->cards = SortedCardCollection();
		}

		std::string StupidLocalPlayerForDebugging::GetName()
		{
			 return this->name; 
		}

		StupidLocalPlayerForDebugging::StupidLocalPlayerForDebugging(std::string name)
		{
			this->name = name;
		}
		
		

	}
}