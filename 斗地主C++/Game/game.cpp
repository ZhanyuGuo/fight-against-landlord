#include "game.h"
#include <memory>
#include <iostream>

namespace PokerGame
{
	namespace FAL
	{


#pragma region GameProcess
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
			// TODO: ���Ƿ�������ѭ���������¼���������ʽ
			do
			{
				this->ResetGame();
				this->HandoutInitCards();
				this->DetermineLandlord();
			} while (!this->LandlordSelected);

			std::cout << "������" << this->LandlordIndex + 1 << "��" << std::endl;
			std::cout << "������" << this->LordCards->ToString() << std::endl;

			for (int i = 0; i < 3; i++)
			{
				std::cout << this->players[i]->GetName() << "������" << this->players[i]->GetCards().ToString() << std::endl;
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

			std::shared_ptr<TypedCardCollection> currentCards(nullptr);
			while (true)
			{
				if (currentState == State::Active)
				{
					TypedCardEvent activeMotionInfo(State::Active);
					auto activeResponse = this->players[idx]->CardResponse(activeMotionInfo);
					if (this->players[idx]->GetCards().Count() == 0)
					{
						std::cout << this->players[idx]->GetName() << "������" << std::endl;
						break;
					}
					currentCards = std::dynamic_pointer_cast<TypedCardCollection>(activeResponse.GetContent());
					currentState = State::Follow;
					passed = 0;
				}
				else
				{
					TypedCardEvent followMotionInfo(State::Follow, currentCards);
					auto followResponse = this->players[idx]->CardResponse(followMotionInfo);
					if (this->players[idx]->GetCards().Count() == 0)
					{
						std::cout << this->players[idx]->GetName() << "������" << std::endl;
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
						currentCards = std::dynamic_pointer_cast<TypedCardCollection>(followResponse.GetContent());
						passed = 0;
						break;
					}

				}
				idx = (idx + 1) % 3;
			}
		}
#pragma endregion

#pragma region CardEvent
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
#pragma endregion

#pragma region TypedCardEvent

		std::shared_ptr<PokerCardCollection> TypedCardEvent::GetContent() const
		{
			return this->content;
		}

		std::shared_ptr<TypedCardCollection> TypedCardEvent::GetTypedContent() const
		{
			return std::dynamic_pointer_cast<TypedCardCollection>(this->content);
		}

		TypedCardEvent::TypedCardEvent(MotionType type) : CardEvent(type)
		{
			this->content = std::shared_ptr<TypedCardCollection>(nullptr);
		}

		TypedCardEvent::TypedCardEvent(MotionType type, std::shared_ptr<TypedCardCollection>& content) : CardEvent(type)
		{
			this->content = content;
		}


#pragma endregion

#pragma region Player For Debugging
		int StupidLocalPlayerForDebugging::PrepareResponse(int leastPoint) noexcept
		{
			return 3;
		}

		CardEvent StupidLocalPlayerForDebugging::CardResponse(TypedCardEvent e) noexcept
		{
			int c = this->cards.Count();
			switch (e.GetType())
			{
			case CardEvent::MotionType::Active:
			{
				PokerCard card = this->cards.PickOut(c - 1);
				std::shared_ptr<PokerCardCollection> cl(new DanZhangCollection(card));
				std::cout << this->name << std::string(":�����") << card.ToString(true) << std::endl;
				return CardEvent(CardEvent::MotionType::Follow, cl);
				break;
			}
			case CardEvent::MotionType::Follow:
			{
				if (e.GetTypedContent()->GetGeneralType() != GeneralCardType::Dan)
				{
					std::cout << this->name << std::string(":����") << std::endl;
					return CardEvent(CardEvent::MotionType::Pass);
				}
				PokerCard largestCard = this->cards[c - 1];
				if (largestCard.Get3BasedNum() > (*(e.GetContent()))[0].Get3BasedNum())
				{
					this->cards.PickOut(c - 1);
					std::shared_ptr<PokerCardCollection> cle(new DanZhangCollection(largestCard));
					std::cout << this->name << std::string(":������") << largestCard.ToString(true) << std::endl;
					return CardEvent(CardEvent::MotionType::Follow, cle);
				}
				else
				{
					std::cout << this->name << std::string(":����") << std::endl;
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
#pragma endregion
		



}
}