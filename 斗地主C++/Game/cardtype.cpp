#include "cardtype.h"


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

		std::shared_ptr<TypedCardCollection> DanZhangCollection::FromCollection(PokerCardCollection& collection)
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

#pragma endregion
	}
}

