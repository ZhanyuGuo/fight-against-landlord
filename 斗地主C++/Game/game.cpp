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
		}

		GameProcess::~GameProcess()
		{

		}

		void GameProcess::Run()
		{
			// TODO: 拆分成多个函数
			// TODO: 考虑放弃各种循环，改用事件驱动的形式
			int magnification = 1;
			int landlordIndex = 0;

			for (int i = 0; i < PlayerNum; i++)
			{
				this->players[i]->Reset();
			}
			
			auto cardHeap = TakeOnlyCardCollection::Standard54();

			for (int i = 0; i < 51; i++)
			{
				int playerIndex = i % 3;
				PokerCardCollection& playerCard = this->players[playerIndex]->GetCards();
				cardHeap >> playerCard;
			}

			int startIndex = rand() % 3;
			int leastPoint = 0;
			for (int i = 0; i < PlayerNum; i++)
			{
				int playerIndex = (i + startIndex) % 3;
				int willingness = this->players[playerIndex]->PrepareResponse(leastPoint);
				if (willingness > leastPoint)
				{
					leastPoint = willingness;
					landlordIndex = playerIndex;
					if (leastPoint == 3)
					{
						break;
					}
				}

			}

			if (leastPoint == 0)
			{
				//TODO: 把之前的过程写进循环里，没有人抢地主就重开
			}
			else
			{
				magnification *= leastPoint;
			}

			this->players[landlordIndex]->GetCards() << cardHeap;
			
			int idx = landlordIndex;
			int passed = 0;
			using State = CardEvent::MotionType;
			State currentState = State::Active;
			//TODO: 将currentCards改成智能指针
			PokerCardCollection* currentCards= nullptr;
			while (true)
			{
				if (currentState == State::Active)
				{
					CardEvent activeMotionInfo(State::Active, nullptr);
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

		PokerCardCollection* CardEvent::GetContent() const
		{
			return this->content;
		}

		CardEvent::CardEvent(MotionType type, PokerCardCollection* content)
		{
			this->type = type;
			this->content = content;
		}

		int StupidLocalPlayerForDebugging::PrepareResponse(int leastPoint)
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
				PokerCardCollection* cl = new IdBasedCardCollection();
				*cl << card;
				std::cout << this->name << std::string(":打出了") << card.ToString() << std::endl;
				return CardEvent(CardEvent::MotionType::Follow, cl);
				break;
			}
			case CardEvent::MotionType::Follow:
			{
				PokerCard largestCard = this->cards[c - 1];
				if (largestCard.Get3BasedNum() > (*(e.GetContent()))[(*(e.GetContent())).Count() - 1].Get3BasedNum())
				{
					this->cards.PickOut(c - 1);
					PokerCardCollection* cle = new IdBasedCardCollection();
					*cle << largestCard;
					std::cout << this->name << std::string(":跟随了") << largestCard.ToString() << std::endl;
					return CardEvent(CardEvent::MotionType::Follow, cle);
				}
				else
				{
					std::cout << this->name << std::string(":跳过") << std::endl;
					return CardEvent(CardEvent::MotionType::Pass, nullptr);
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

		StupidLocalPlayerForDebugging::StupidLocalPlayerForDebugging(std::string name)
		{
			this->name = name;
		}

}
}