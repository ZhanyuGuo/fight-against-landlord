#include "cardtype.h"
#include <map>


namespace PokerGame
{
	namespace FAL
	{


#pragma region 单张
		DanZhangCollection::DanZhangCollection(PokerCard card)
		{
			this->cards.push_back(card.GetId());
		}

		DanZhangCollection::DanZhangCollection(PokerCardId id)
		{
			this->cards.push_back(id);
		}

		DanZhangCollection::DanZhangCollection(PokerCardCollection& collection)
		{
			if (collection.Count() == 1)
			{
				*this << collection[0];
			}
			else
			{
				throw NotSameTypeException();
			}
		}

		GeneralCardType DanZhangCollection::GetGeneralType()
		{
			return GeneralCardType::Dan;
		}

		bool DanZhangCollection::IsSameType(TypedCardCollection& other)
		{
			GeneralCardType otherType = other.GetGeneralType();
			return otherType == GeneralCardType::Dan;
		}

		bool DanZhangCollection::IsLargerThan(TypedCardCollection& other)
		{
			if (!this->IsSameType(other))
			{
				return false;
			}
			else
			{
				DanZhangCollection* otherDan = dynamic_cast<DanZhangCollection*>(&other);
				PokerCard selfCard(this->cards[0]), otherCard(otherDan->cards[0]);
				PokerPoint selfNum = selfCard.Get3BasedNum(), otherNum = otherCard.Get3BasedNum();
				return selfNum > otherNum;
			}			
		}

		std::shared_ptr<TypedCardCollection> DanZhangCollection::FormatCollection(PokerCardCollection& collection)
		{
			if (collection.Count() != 1)
			{
				throw NotSameTypeException();
			}
			else
			{
				std::shared_ptr<TypedCardCollection> danCollection(new DanZhangCollection(collection[0]));
				return danCollection;
			}
		}
#pragma endregion

#pragma region 对子

		GeneralCardType DuiZiColleciton::GetGeneralType()
		{
			return GeneralCardType::Dui;
		}

		bool DuiZiColleciton::IsSameType(TypedCardCollection& other)
		{
			GeneralCardType type = other.GetGeneralType();
			return type == GeneralCardType::Dui;
		}

		bool DuiZiColleciton::IsLargerThan(TypedCardCollection& other)
		{
			if (!this->IsSameType(other))
			{
				return false;
			}
			else
			{
				DuiZiColleciton* otherDui = dynamic_cast<DuiZiColleciton*>(&other);
				PokerPoint selfNum = this->Get3BasedNum(),
					otherNum = otherDui->Get3BasedNum();
				return selfNum > otherNum;
			}
		}

		std::shared_ptr<TypedCardCollection> DuiZiColleciton::FormatCollection(PokerCardCollection& collection)
		{
			try
			{
				DuiZiColleciton* dui = new DuiZiColleciton(collection);
				return std::shared_ptr<DuiZiColleciton>(dui);
			}
			catch (NotSameTypeException)
			{
				throw;
			}
		}

		PokerPoint DuiZiColleciton::Get3BasedNum()
		{
			return PokerCard(this->cards[0]).Get3BasedNum();
		}

		DuiZiColleciton::DuiZiColleciton(PokerCardCollection& collection)
		{
			if (collection.Count() != 2)
			{
				throw NotSameTypeException();
			}
			else
			{
				PokerCard card0 = collection[0],
					card1 = collection[1];
				PokerPoint point0 = card0.GetPoint(),
					point1 = card1.GetPoint();
				if (point0 >= 1 && point0 <= 13 && point0 == point1)
				{
					*this << card0 << card1;
				}
				else
				{
					throw NotSameTypeException();
				}
			}
		}


#pragma endregion
		
#pragma region 三张

		GeneralCardType SanZhangCollection::GetGeneralType() 
		{
			return GeneralCardType::San;
		}

		bool SanZhangCollection::IsSameType(TypedCardCollection& other)
		{
			GeneralCardType type = other.GetGeneralType();
			if (type != GeneralCardType::San)
			{
				return false;
			}
			else
			{
				SanZhangCollection* otherSan = dynamic_cast<SanZhangCollection*>(&other);
				return this->attachedCount == otherSan->attachedCount;
			}
		}

		bool SanZhangCollection::IsLargerThan(TypedCardCollection& other)
		{
			if (!this->IsSameType(other))
			{
				return false;
			}
			else
			{
				SanZhangCollection* otherSan = dynamic_cast<SanZhangCollection*>(&other);
				PokerPoint self3BasedNum = PokerCard::NormalPointTo3BasedNum(this->mainPoint),
					other3BasedNum = PokerCard::NormalPointTo3BasedNum(otherSan->mainPoint);
				return self3BasedNum > other3BasedNum;
			}
		}

		std::shared_ptr<TypedCardCollection> SanZhangCollection::FormatCollection(PokerCardCollection& collection)
		{
			try
			{
				SanZhangCollection* san = new SanZhangCollection(collection);
				return std::shared_ptr<SanZhangCollection>(san);
			}
			catch(NotSameTypeException)
			{
				throw;
			}
		}

		SanZhangCollection::SanZhangCollection(PokerCardCollection& collection)
		{
			std::map<PokerPoint, int> cardRepitition;
			for (int i = 0; i < collection.Count(); i++)
			{
				PokerCard card = collection[i];
				PokerPoint point = card.GetPoint();
				cardRepitition[point] += 1;
			}
			if (cardRepitition.size() == 1) 
			{
				auto item = cardRepitition.begin();
				if (item->second == 3)
				{
					this->mainPoint = item->first;
					this->attachedCount = 0;
				}
			}
			else if (cardRepitition.size() == 2)
			{
				bool gotMain = false, gotAttached = false;
				for (auto pair : cardRepitition)
				{
					if (pair.second == 3)
					{
						if (!gotMain)
						{
							this->mainPoint = pair.first;
							gotMain = true;
						}
						else
						{
							throw NotSameTypeException();
						}
					}
					else if (pair.second == 2 || pair.second == 1)
					{
						if (!gotAttached)
						{
							this->attachedPoint = pair.first;
							this->attachedCount = pair.second;
							gotAttached = true;
						}
						else
						{
							throw NotSameTypeException();
						}

					}				
				}
				if (!(gotMain && gotAttached))
				{
					throw NotSameTypeException();
				}
			}
			else
			{
				throw NotSameTypeException();
			}
		}


#pragma endregion
	}
}

