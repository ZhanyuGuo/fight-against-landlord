#include "cardtype.h"
#include <map>
#include <algorithm>


namespace PokerGame
{
	namespace FAL
	{


#pragma region ����
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

#pragma region ����

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

#pragma region ����

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
				if (!this->IsSameType(*san))
				{
					delete san;
					throw NotSameTypeException();
				}
				return std::shared_ptr<SanZhangCollection>(san);
			}
			catch (NotSameTypeException)
			{
				throw;
			}
		}

		SanZhangCollection::SanZhangCollection(PokerCardCollection& collection)
		{
			CardRepetitionDict cardRepitition;
			cardRepitition = PokerCardCollection::GetCardRepetition(collection);
			if (cardRepitition.size() == 1)
			{
				auto item = cardRepitition.begin();
				if (item->second == 3)
				{
					this->mainPoint = item->first;
					this->attachedCount = 0;
				}
				else
				{
					throw NotSameTypeException();
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
				else
				{
					*this << collection;
				}
			}
			else
			{
				throw NotSameTypeException();
			}
		}


#pragma endregion

#pragma region ˳��
		GeneralCardType ShunZiCollection::GetGeneralType()
		{
			return GeneralCardType::Shun;
		}

		bool ShunZiCollection::IsSameType(TypedCardCollection& other)
		{
			if (other.GetGeneralType() != GeneralCardType::Shun)
			{
				return false;
			}
			else
			{
				return this->Count() == other.Count();
			}
		}

		bool ShunZiCollection::IsLargerThan(TypedCardCollection& other)
		{
			if (!this->IsSameType(other))
			{
				return false;
			}
			else
			{
				ShunZiCollection* otherShun = dynamic_cast<ShunZiCollection*>(&other);
				return this->upper3BasedPoint > otherShun->upper3BasedPoint;
			}
		}

		std::shared_ptr<TypedCardCollection> ShunZiCollection::FormatCollection(PokerCardCollection& collection)
		{
			try
			{
				ShunZiCollection* shun = new ShunZiCollection(collection);
				if (!this->IsSameType(*shun))
				{
					delete shun;
					throw NotSameTypeException();
				}
				return std::shared_ptr<ShunZiCollection>(shun);
			}
			catch (NotSameTypeException)
			{
				throw;
			}
		}

		ShunZiCollection::ShunZiCollection(PokerCardCollection& collection)
		{
			CardRepetitionDict repetition = collection.GetCardRepetition(collection);
			std::vector<PokerPoint> card3BasedNums;
			for (auto pair : repetition)
			{
				if (pair.second != 1)
				{
					throw NotSameTypeException();
				}
				else
				{
					PokerPoint threeBaseNum = PokerCard::NormalPointTo3BasedNum(pair.first);
					if (threeBaseNum <= 12)
					{
						card3BasedNums.push_back(threeBaseNum);
					}
					else
					{
						throw NotSameTypeException();
					}
				}
			}
			std::sort(card3BasedNums.begin(), card3BasedNums.end());
			int count = card3BasedNums.size();
			if (count < 5)
			{
				throw NotSameTypeException();
			}
			int dif = card3BasedNums[count - 1] - card3BasedNums[0];
			if ((count - dif) == 1)
			{
				*this << collection;
				std::sort(this->cards.begin(), this->cards.end(), FAL_IsSmallerCard());
				this->lower3BasedPoint = card3BasedNums[0];
				this->upper3BasedPoint = this->lower3BasedPoint + dif;
			}
			else
			{
				throw NotSameTypeException();
			}
		}
#pragma endregion

#pragma region ը��
		GeneralCardType ZhaDanCollection::GetGeneralType()
		{
			return GeneralCardType::Zha;
		}

		bool ZhaDanCollection::IsSameType(TypedCardCollection& other)
		{
			return other.GetGeneralType() == GeneralCardType::Zha;
		}

		bool ZhaDanCollection::IsLargerThan(TypedCardCollection& other)
		{
			switch (other.GetGeneralType())
			{
			case GeneralCardType::Zha:
			{
				ZhaDanCollection* otherZha = dynamic_cast<ZhaDanCollection*>(&other);
				return this->threeBasedNum > otherZha->threeBasedNum;
			}
			case GeneralCardType::LianZha:
			{
				return false;
			}
			case GeneralCardType::WangZha:
			{
				return false;
			}
			default:
				return true;
				break;
			}
		}

		std::shared_ptr<TypedCardCollection> ZhaDanCollection::FormatCollection(PokerCardCollection& collection)
		{
			try
			{
				ZhaDanCollection* zha = new ZhaDanCollection(collection);
				return std::shared_ptr<ZhaDanCollection>(zha);
			}
			catch (NotSameTypeException)
			{
				throw;
			}
		}

		ZhaDanCollection::ZhaDanCollection(PokerCardCollection& collection)
		{
			if (collection.Count() != 4)
			{
				throw NotSameTypeException();
			}
			else
			{
				CardRepetitionDict repetition = PokerCardCollection::GetCardRepetition(collection);
				if (repetition.size() != 1)
				{
					throw NotSameTypeException();
				}
				else
				{
					PokerPoint point = repetition.begin()->first;
					this->threeBasedNum = PokerCard::NormalPointTo3BasedNum(point);
					*this << collection;
				}
			}
		}
#pragma endregion

#pragma region ��ը
		GeneralCardType WangZhaCollection::GetGeneralType()
		{
			return GeneralCardType::WangZha;
		}

		bool WangZhaCollection::IsSameType(TypedCardCollection& other)
		{
			return other.GetGeneralType() == GeneralCardType::WangZha;
		}

		bool WangZhaCollection::IsLargerThan(TypedCardCollection& other)
		{
			switch (other.GetGeneralType())
			{
			case GeneralCardType::WangZha:
				return false;
			case GeneralCardType::LianZha:
				return false;
			default:
				return true;
				break;
			}
		}

		std::shared_ptr<TypedCardCollection> WangZhaCollection::FormatCollection(PokerCardCollection& collection)
		{
			try
			{
				WangZhaCollection* wangZha = new WangZhaCollection(collection);
				return std::shared_ptr<WangZhaCollection>(wangZha);
			}
			catch(NotSameTypeException)
			{
				throw;
			}
			
		}

		WangZhaCollection::WangZhaCollection(PokerCardCollection& collection)
		{
			if (collection.Count() != 2)
			{
				throw NotSameTypeException();
			}
			else
			{
				bool got1 = false, got2 = false;
				for (int i = 0; i < 2; i++)
				{
					if (collection[i].GetPoint() == 14)
					{
						got1 = true;
					}
					else if (collection[i].GetPoint() == 15)
					{
						got2 = true;
					}
				}
				if (got1 && got2)
				{
					*this << collection;
				}
				else
				{
					throw NotSameTypeException();
				}
			}
		}
#pragma endregion

#pragma region ����
		GeneralCardType LianDuiCollection::GetGeneralType()
		{
			return GeneralCardType::LianDui;
		}

		bool LianDuiCollection::IsSameType(TypedCardCollection& other)
		{
			if (other.GetGeneralType() != GeneralCardType::LianDui)
			{
				return false;
			}
			else
			{
				LianDuiCollection* otherLianDui = dynamic_cast<LianDuiCollection*>(&other);
				int thisSpan = this->upper3BasedPoint - this->lower3BasedPoint;
				int otherSpan = otherLianDui->upper3BasedPoint - otherLianDui->lower3BasedPoint;
				return thisSpan == otherSpan;
			}
		}

		bool LianDuiCollection::IsLargerThan(TypedCardCollection& other)
		{
			if (!this->IsSameType(other))
			{
				return false;
			}
			else
			{
				LianDuiCollection* otherLianDui = dynamic_cast<LianDuiCollection*>(&other);
				return this->upper3BasedPoint > otherLianDui->upper3BasedPoint;
			}
			return false;
		}

		std::shared_ptr<TypedCardCollection> LianDuiCollection::FormatCollection(PokerCardCollection& collection)
		{
			return std::shared_ptr<TypedCardCollection>();
		}

		LianDuiCollection::LianDuiCollection(PokerCardCollection& collection)
		{
			CardRepetitionDict repetition = collection.GetCardRepetition(collection);
			std::vector<PokerPoint> card3BasedNums;
			for (auto pair : repetition)
			{
				if (pair.second != 2)
				{
					throw NotSameTypeException();
				}
				else
				{
					PokerPoint threeBaseNum = PokerCard::NormalPointTo3BasedNum(pair.first);
					if (threeBaseNum <= 12)
					{
						card3BasedNums.push_back(threeBaseNum);
					}
					else
					{
						throw NotSameTypeException();
					}
				}
			}
			std::sort(card3BasedNums.begin(), card3BasedNums.end());
			int count = card3BasedNums.size();
			if (count < 3)
			{
				throw NotSameTypeException();
			}
			int dif = card3BasedNums[count - 1] - card3BasedNums[0];
			if ((count - dif) == 1)
			{
				*this << collection;
				std::sort(this->cards.begin(), this->cards.end(), FAL_IsSmallerCard());
				this->lower3BasedPoint = card3BasedNums[0];
				this->upper3BasedPoint = this->lower3BasedPoint + dif;
			}
			else
			{
				throw NotSameTypeException();
			}
		}
#pragma endregion





#pragma region ���ͳ��ԣ��ݶ���
		TypedCardCollection& TypedCardCollection::TryCast(PokerCardCollection& collection)
		{
			TypedCardCollection* typedCol = nullptr;
			try { typedCol = new DanZhangCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new DuiZiColleciton(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new SanZhangCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new ShunZiCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new LianDuiCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new ZhaDanCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new WangZhaCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			throw InvalidTypeException();
		}

		TypedCardCollection& TypedCardCollection::TryCastZhaDanOnly(PokerCardCollection& collection)
		{
			TypedCardCollection* typedCol = nullptr;

			try { typedCol = new ZhaDanCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }
			try { typedCol = new WangZhaCollection(collection); return *typedCol; }
			catch (NotSameTypeException) { typedCol = nullptr; }

			throw InvalidTypeException();
		}
#pragma endregion




		

}
}

