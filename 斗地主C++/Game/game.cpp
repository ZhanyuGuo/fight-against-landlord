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
			int willingness = 0;
			this->LandlordSelected = false;
			for (int i = 0; i < PlayerNum; i++)
			{
				int playerIndex = (i + startIndex) % 3;
				willingness = this->players[playerIndex]->PrepareResponse(leastPoint);
				if (willingness > leastPoint)
				{
					leastPoint = willingness;
					this->LandlordSelected = true;
					this->LandlordIndex = playerIndex;
					if (leastPoint == 3)
					{
						break;
					}
				}
				else
				{
					leastPoint += 1;
				}
			}
			if (this->LandlordSelected)
			{
				this->Magnification *= leastPoint;
				this->players[this->LandlordIndex]->GetCards() << *(this->LordCards);
			}
			std::string temp;
			std::getline(std::cin, temp);
		}

		void GameProcess::MainCardLoop()
		{
			int idx = this->LandlordIndex;
			int passed = 0;
			using State = CardEventArgs::MotionType;
			State currentState = State::Active;

			std::shared_ptr<TypedCardCollection> currentCards(nullptr);
			while (true)
			{
				if (currentState == State::Active)
				{
					TypedCardEventArgs activeMotionInfo(State::Active);
					auto activeResponse = this->players[idx]->CardResponse(activeMotionInfo);
					if (this->players[idx]->GetCards().Count() == 0)
					{
						std::cout << this->players[idx]->GetName() << "打完了" << std::endl;
						break;
					}
					currentCards = std::dynamic_pointer_cast<TypedCardCollection>(activeResponse.GetContent());
					currentState = State::Follow;
					passed = 0;
				}
				else
				{
					TypedCardEventArgs followMotionInfo(State::Follow, currentCards);
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
						currentCards = std::dynamic_pointer_cast<TypedCardCollection>(followResponse.GetContent());
						passed = 0;
						break;
					}

				}
				idx = (idx + 1) % 3;
			}
		}
#pragma endregion

#pragma region CardEventArgs
		CardEventArgs::MotionType CardEventArgs::GetType() const
		{
			return this->type;
		}

		std::shared_ptr<PokerCardCollection> CardEventArgs::GetContent() const
		{
			return this->content;
		}

		CardEventArgs::CardEventArgs(MotionType type)
		{
			this->type = type;
			this->content = std::shared_ptr<PokerCardCollection>(nullptr);
		}

		CardEventArgs::CardEventArgs(MotionType type, std::shared_ptr<PokerCardCollection>& content)
		{
			this->type = type;
			this->content = content;
		}
#pragma endregion

#pragma region TypedCardEventArgs

		std::shared_ptr<PokerCardCollection> TypedCardEventArgs::GetContent() const
		{
			return this->content;
		}

		std::shared_ptr<TypedCardCollection> TypedCardEventArgs::GetTypedContent() const
		{
			return std::dynamic_pointer_cast<TypedCardCollection>(this->content);
		}

		TypedCardEventArgs::TypedCardEventArgs(MotionType type) : CardEventArgs(type)
		{
			this->content = std::shared_ptr<TypedCardCollection>(nullptr);
		}

		TypedCardEventArgs::TypedCardEventArgs(MotionType type, std::shared_ptr<TypedCardCollection>& content) : CardEventArgs(type)
		{
			this->content = content;
		}


#pragma endregion

#pragma region Player For Debugging
		int StupidLocalPlayerForDebugging::PrepareResponse(int leastPoint) noexcept
		{
			return 3;
		}

		CardEventArgs StupidLocalPlayerForDebugging::CardResponse(TypedCardEventArgs e) noexcept
		{
			int c = this->cards.Count();
			switch (e.GetType())
			{
			case CardEventArgs::MotionType::Active:
			{
				PokerCard card = this->cards.PickOut(c - 1);
				std::shared_ptr<PokerCardCollection> cl(new DanZhangCollection(card));
				std::cout << this->name << std::string(":打出了") << card.ToString(true) << std::endl;
				return CardEventArgs(CardEventArgs::MotionType::Follow, cl);
				break;
			}
			case CardEventArgs::MotionType::Follow:
			{
				if (e.GetTypedContent()->GetGeneralType() != GeneralCardType::Dan)
				{
					std::cout << this->name << std::string(":跳过") << std::endl;
					return CardEventArgs(CardEventArgs::MotionType::Pass);
				}
				PokerCard largestCard = this->cards[c - 1];
				if (largestCard.Get3BasedNum() > (*(e.GetContent()))[0].Get3BasedNum())
				{
					this->cards.PickOut(c - 1);
					std::shared_ptr<PokerCardCollection> cle(new DanZhangCollection(largestCard));
					std::cout << this->name << std::string(":跟随了") << largestCard.ToString(true) << std::endl;
					return CardEventArgs(CardEventArgs::MotionType::Follow, cle);
				}
				else
				{
					std::cout << this->name << std::string(":跳过") << std::endl;
					return CardEventArgs(CardEventArgs::MotionType::Pass);
				}
				break;
			}
			default:
			{
				return CardEventArgs(CardEventArgs::MotionType::Pass);
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

#pragma region 手动本地玩家
		int ManualLocalPlayer::PrepareResponse(int leastPoint) noexcept
		{
			std::cout << this->name << ":请输入抢地主分数,最高3分,且不低于" << leastPoint + 1 << "分,否则视为放弃:";
			int point;
			std::cin >> point;
			if (point > leastPoint && point <= 3)
			{
				return point;
			}
			else
			{
				return 0;
			}
		}

		CardEventArgs ManualLocalPlayer::CardResponse(TypedCardEventArgs e) noexcept
		{
			switch (e.GetType())
			{
			case CardEventArgs::MotionType::Active:
			{
				std::cout << this->name << ":轮到自己出牌（主动）" << std::endl;
				std::cout << this->name << ":手牌:" << this->SelfCardWithIndex() << std::endl;
				bool gotValidCard = false;
				IdBasedCardCollection rawCollection;
				std::shared_ptr<TypedCardCollection> typedCollection;
				do
				{
					this->cards << rawCollection;
					std::cout << this->name << ":输入牌的索引（空格分隔），输入回车确认：";
					rawCollection = this->ReadCollection();
					try
					{
						typedCollection = std::shared_ptr<TypedCardCollection>(&TypedCardCollection::TryCast(rawCollection));
						gotValidCard = true;
					}
					catch (InvalidTypeException)
					{
						std::cout << this->name << ":Invalid Card Type,请重新输入" << std::endl;
					}
				} while (!gotValidCard);
				return TypedCardEventArgs(CardEventArgs::MotionType::Active, typedCollection);
			}
			case CardEventArgs::MotionType::Follow:
			{
				std::cout << this->name << ":轮到自己出牌（跟随）" << std::endl;
				std::cout << this->name << ":上家出牌:" << e.GetContent()->ToString() << std::endl;
				std::cout << this->name << ":手牌:" << this->SelfCardWithIndex() << std::endl;
				bool gotValidCard = false;
				IdBasedCardCollection rawCollection;
				std::shared_ptr<TypedCardCollection> typedCollection;
				do
				{
					this->cards << rawCollection;
					std::cout << this->name << ":输入牌的索引（空格分隔），输入回车确认，直接回车以跳过：";
					rawCollection = this->ReadCollection();
					if (rawCollection.Count() == 0)
					{
						return TypedCardEventArgs(CardEventArgs::MotionType::Pass);
					}
					try
					{
						auto formerCollection = e.GetTypedContent();
						try
						{
							typedCollection = formerCollection->FormatCollection(rawCollection);
						}
						catch (NotSameTypeException)
						{
							typedCollection = std::shared_ptr<TypedCardCollection>(&TypedCardCollection::TryCast(rawCollection));
						}
						if (typedCollection->IsLargerThan(*formerCollection))
						{
							gotValidCard = true;
						}
						else
						{
							std::cout << this->name << ":该牌不能压过上家,请重新输入" << std::endl;
						}
					}
					catch (InvalidTypeException)
					{
						std::cout << this->name << ":Invalid Card Type,请重新输入" << std::endl;
					}
				} while (!gotValidCard);
				return TypedCardEventArgs(CardEventArgs::MotionType::Follow, typedCollection);
			}
			default:
				return TypedCardEventArgs(CardEventArgs::MotionType::Pass);
				break;
			}
		}

		PokerCardCollection& ManualLocalPlayer::GetCards()
		{
			return this->cards;
		}

		std::string ManualLocalPlayer::GetName()
		{
			return this->name;
		}

		void ManualLocalPlayer::Reset()
		{
			this->cards = SortedCardCollection();
		}

		ManualLocalPlayer::ManualLocalPlayer(std::string name)
		{
			this->name = name;
		}

		std::string ManualLocalPlayer::SelfCardWithIndex()
		{
			std::string indexedCards("");
			int count = this->cards.Count();
			for (int i = 0; i < count; i++)
			{
				indexedCards += std::string("[");
				indexedCards += std::to_string(i);
				indexedCards += std::string("]");
				indexedCards += this->cards[i].ToString(true);
				indexedCards += std::string(",");
			}
			return indexedCards;
		}

		IdBasedCardCollection ManualLocalPlayer::ReadCollection()
		{
			std::string line;
			char buf[128];
			std::cin.getline(buf, 128);
			//std::getline(std::cin, line);
			line = std::string(buf);
			//std::vector<int> selectedIndexes;
			size_t startPos = 0, nextSpacePos;
			nextSpacePos = line.find(' ', startPos);
			IdBasedCardCollection rawCollection;
			std::string subStr;
			while (nextSpacePos != std::string::npos)
			{
				subStr = line.substr(startPos, nextSpacePos - startPos);
				try
				{

					if (!subStr.empty())
					{
						int index = std::stoi(subStr);
						rawCollection << this->cards[index];
					}
				}
				catch (std::invalid_argument)
				{
					std::cout << this->name << ":无法转化为索引的输入，该项被忽略:" << subStr << std::endl;
				}
				catch (std::out_of_range e)
				{
					std::cout << this->name << ":非法的索引项，该项被忽略:" << e.what() << std::endl;
				}
				startPos = nextSpacePos + 1;
				nextSpacePos = line.find(' ', startPos);
			}
			subStr = line.substr(startPos);
			try
			{
				if (!subStr.empty())
				{
					int index = std::stoi(subStr);
					rawCollection << this->cards[index];
				}
			}
			catch (std::invalid_argument)
			{
				std::cout << this->name << ":无法转化为索引的输入，该项被忽略:" << subStr << std::endl;
			}
			catch (std::out_of_range e)
			{
				std::cout << this->name << ":非法的索引项，该项被忽略:" << e.what() << std::endl;
			}
			this->cards.PickOut(rawCollection);
			return rawCollection;
		}

#pragma endregion




	}
}