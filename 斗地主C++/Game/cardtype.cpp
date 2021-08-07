#include "cardtype.h"
#include <map>
#include <algorithm>


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
					*this << collection;
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

#pragma region 顺子
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

#pragma region 炸弹
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

#pragma region 王炸
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

#pragma region 连对
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

#pragma region 飞机
		GeneralCardType FeiJiCollection::GetGeneralType()
		{
			return GeneralCardType::FeiJi;
		}

		bool FeiJiCollection::IsSameType(TypedCardCollection& other)
		{
			if (other.GetGeneralType() != GeneralCardType::FeiJi)
			{
				return false;
			}
			else
			{
				FeiJiCollection* otherFeiJi = dynamic_cast<FeiJiCollection*>(&other);
				int thisSpan = this->upper3BasedMainPoint - this->lower3BasedMainPoint;
				int otherSpan = otherFeiJi->upper3BasedMainPoint - otherFeiJi->lower3BasedMainPoint;
				if (thisSpan != otherSpan)
				{
					return false;
				}
				else
				{
					return this->attachedUnitSize == otherFeiJi->attachedUnitSize;
				}
			}
			return false;
		}

		bool FeiJiCollection::IsLargerThan(TypedCardCollection& other)
		{
			if (!this->IsSameType(other))
			{
				return false;
			}
			else
			{
				FeiJiCollection* otherFeiJi = dynamic_cast<FeiJiCollection*>(&other);
				return this->upper3BasedMainPoint > otherFeiJi->upper3BasedMainPoint;
			}
		}

		std::shared_ptr<TypedCardCollection> FeiJiCollection::FormatCollection(PokerCardCollection& collection)
		{
			try
			{
				FeiJiCollection* feiJi = new FeiJiCollection(collection);
				if (!this->IsSameType(*feiJi))
				{
					delete feiJi;
					throw NotSameTypeException();
				}
				return std::shared_ptr<FeiJiCollection>(feiJi);
			}
			catch (NotSameTypeException)
			{
				throw;
			}
			
		}


		FeiJiCollection::FeiJiCollection(PokerCardCollection& collection)
		{
#ifdef Feiji_Constructor_Old
			CardRepetitionDict repetition = collection.GetCardRepetition(collection);
			std::vector<PokerPoint> main3BasedNums;
			bool attachedUnitSizeDetermined = false;

			for (auto pair : repetition)
			{
				if (pair.second == 3)
				{
					PokerPoint main3BasedNum = PokerCard::NormalPointTo3BasedNum(pair.first);
					if (main3BasedNum >= 1 && main3BasedNum <= 12)
					{
						main3BasedNums.push_back(main3BasedNum);
					}
					else
					{
						throw NotSameTypeException();
					}
				}
				else if (pair.second == 1 || pair.second == 2)
				{
					if (!attachedUnitSizeDetermined)
					{
						this->attachedUnitSize = pair.second;
						attachedUnitSizeDetermined = true;
						this->attachedPoints.push_back(pair.first);
					}
					else
					{
						if (pair.second != this->attachedUnitSize)
						{
							throw NotSameTypeException();
						}
						else
						{
							this->attachedPoints.push_back(pair.first);
						}
					}
				}
				else
				{
					throw NotSameTypeException();
				}	
			}
			if (!attachedUnitSizeDetermined)
			{
				this->attachedUnitSize = 0;
			}
			else
			{
				if (this->attachedPoints.size() != main3BasedNums.size())
				{
					throw NotSameTypeException();
				}
			}
			std::sort(main3BasedNums.begin(), main3BasedNums.end());
			int count = main3BasedNums.size();
			if (count < 2)
			{
				throw NotSameTypeException();
			}
			int dif = main3BasedNums[count - 1] - main3BasedNums[0];
			if ((count - dif) != 1)
			{
				throw NotSameTypeException();
			}
			else
			{
				*this << collection;
				this->lower3BasedMainPoint = main3BasedNums[0];
				this->upper3BasedMainPoint = main3BasedNums[count - 1];
			}
#else
			CardRepetitionDict repetition = PokerCardCollection::GetCardRepetition(collection);
			std::vector<PokerPoint> threeBasedNums;

			for (auto pair : repetition)
			{
				if (pair.second >= 3)
				{
					PokerPoint threeBaseNum = PokerCard::NormalPointTo3BasedNum(pair.first);
					if (threeBaseNum >= 1 && threeBaseNum <= 12)
					{
						threeBasedNums.push_back(threeBaseNum);
					}
				}
			}
			if (threeBasedNums.size() < 2)
			{
				throw NotSameTypeException();
			}
			auto subSeqs = this->GetContinousSubSeq(threeBasedNums);
			if (subSeqs.size() == 0)
			{
				throw NotSameTypeException();
			}
			else
			{
				for (auto continous3BasedNums : subSeqs)
				{
					int mainPairCount = continous3BasedNums.size();
					int mainCount = 3 * mainPairCount;
					int attachedCount = collection.Count() - mainCount;
					if (attachedCount % mainPairCount != 0)
					{
						goto NotSuitableAttached;
					}
					else
					{
						switch (attachedCount / mainPairCount)
						{
						case 0:
						{
							this->attachedUnitSize = 0;
							this->lower3BasedMainPoint = continous3BasedNums[0];
							this->upper3BasedMainPoint = continous3BasedNums[mainPairCount - 1];
							*this << collection;
							IdBasedCardCollection* mainCollection = new IdBasedCardCollection();
							IdBasedCardCollection temp = this->ExtractMainCollection(continous3BasedNums, collection);
							*mainCollection << temp;
							this->mainCache = std::unique_ptr<PokerCardCollection>(mainCollection);
							this->attachedCache = std::unique_ptr<PokerCardCollection>(nullptr);
							return;
							break;
						}
						case 1:
						{
							this->attachedUnitSize = 1;
							this->lower3BasedMainPoint = continous3BasedNums[0];
							this->upper3BasedMainPoint = continous3BasedNums[mainPairCount - 1];
							*this << collection;
							IdBasedCardCollection* mainCollection = new IdBasedCardCollection();
							IdBasedCardCollection temp = this->ExtractMainCollection(continous3BasedNums, collection);
							*mainCollection << temp;
							this->mainCache = std::unique_ptr<PokerCardCollection>(mainCollection);
							IdBasedCardCollection collectionCopy;
							collectionCopy << collection;
							PokerCardCollection* attatchedCollection = &collectionCopy.PickOut(*this->mainCache);
							this->attachedCache = std::unique_ptr<PokerCardCollection>(attatchedCollection);
							//
							return;
							break;
						}
						case 2:
						{
							this->attachedUnitSize = 2;
							this->lower3BasedMainPoint = continous3BasedNums[0];
							this->upper3BasedMainPoint = continous3BasedNums[mainPairCount - 1];
							*this << collection;
							IdBasedCardCollection* mainCollection = new IdBasedCardCollection();
							IdBasedCardCollection temp = this->ExtractMainCollection(continous3BasedNums, collection);
							*mainCollection << temp;
							this->mainCache = std::unique_ptr<PokerCardCollection>(mainCollection);
							IdBasedCardCollection* collectionCopy = new IdBasedCardCollection();
							*collectionCopy << collection;
							collectionCopy->PickOut(*this->mainCache);
							PokerCardCollection* attatchedCollection = static_cast<PokerCardCollection*>(collectionCopy);
							if (!this->IsPaired(*attatchedCollection))
							{
								delete collectionCopy;
								goto NotSuitableAttached;
							}
							this->attachedCache = std::unique_ptr<PokerCardCollection>(attatchedCollection);
							return;
							break;
						}
						default:
						{
							goto NotSuitableAttached;
						}
						}
					}
				NotSuitableAttached:
					{
						continue;
					}
				}
				throw NotSameTypeException();
			}
#endif // Feiji_Constructor_Old
		}

		IdBasedCardCollection FeiJiCollection::ExtractMainCollection(std::vector<PokerPoint> main3BasedNums, PokerCardCollection& collection)
		{
			IdBasedCardCollection mainCollection;
			for (auto main3BasedNum : main3BasedNums)
			{
				int count = 0;
				for (int i = 0; i < collection.Count(); i++)
				{
					PokerPoint threeBasedNum = collection[i].Get3BasedNum();
					if (threeBasedNum == main3BasedNum)
					{
						count++;
						mainCollection << collection[i];
						if (count == 3)
						{
							break;
						}
					}
				}
				// if(count != 3) //exception
			}
			return mainCollection;
		}

		/// <summary>
		/// 获取连续子列,仅FeiJiCollection内部适用,因为具有如下特性:要求seq的值是不重复的;
		/// 子列的长度范围为2至seq.size()(均包含);每种长度的子列只返回值较大的一个;
		/// 较长的子列排在前面
		/// </summary>
		/// <param name="seq"></param>
		/// <returns></returns>
		std::vector<std::vector<PokerPoint>> FeiJiCollection::GetContinousSubSeq(std::vector<PokerPoint> seq)
		{
			std::sort(seq.begin(), seq.end());
			std::vector<std::vector<PokerPoint>> result;
			int iFront, iBack;
			int totalRound = seq.size() - 1;
			for (int round = totalRound; round >= 1 ; round--)
			{
				iBack = seq.size() - 1;
				iFront = iBack - round;
				while (iFront >= 0)
				{
					int indexDif = round;
					int valueDif = seq[iBack] - seq[iFront];
					if (indexDif == valueDif)
					{
						std::vector<PokerPoint> continousSubSeq(seq.begin() + iFront, seq.begin() + iBack + 1);
						result.push_back(continousSubSeq);
						break;
					}
					iBack--;
					iFront--;
				}
			}
			return result;
		}

		bool FeiJiCollection::IsPaired(PokerCardCollection& collection)
		{
			CardRepetitionDict repetition = PokerCardCollection::GetCardRepetition(collection);
			for (auto pair : repetition)
			{
				if (pair.second % 2 != 0)
				{
					return false;
				}
			}
			return true;
		}
#pragma endregion

#pragma region 造型尝试（暂定）
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
			try { typedCol = new FeiJiCollection(collection); return *typedCol; }
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

