#include "poker.h"
#include <algorithm>
#include <random>
#include <time.h>
#include <stdexcept>

namespace PokerGame
{

#pragma region PokerCard
	PokerCard::PokerCard()
	{
		this->id = 0;
	}
	PokerCard::PokerCard(PokerCardId id)
	{
		this->id = id;
	}
	PokerCard::PokerCard(PokerColor color, PokerPoint point)
	{
		if (point == 14 || point == 15)
		{
			this->id = point + 39;
		}
		else
		{
			int color_num = static_cast<int>(color);
			this->id = (color_num - 1) * 13 + point;
		}
	}
	PokerCardId PokerCard::GetId() const
	{
		return this->id;
	}
	PokerPoint PokerCard::GetPoint() const
	{
		if (this->id == 53 || this->id == 54)
		{
			return this->id - 39;
		}
		else
		{
			return (this->id - 1) % 13 + 1;
		}
	}
	PokerPoint PokerCard::Get3BasedNum() const
	{
		PokerPoint point = this->GetPoint();
		return NormalPointTo3BasedNum(point);
	}

	PokerPoint PokerCard::NormalPointTo3BasedNum(PokerPoint point)
	{
		if (point == 14 || point == 15)
		{
			return point;
		}
		else if (point == 1 || point == 2)
		{
			return point + 11;
		}
		else
		{
			return point - 11;
		}
	}

	std::string PokerCard::ToString() const
	{
		switch (this->GetColor())
		{
		case PokerColor::Club:
			return std::string("梅花") + std::to_string(this->GetPoint());
		case PokerColor::Diamond:
			return std::string("方片") + std::to_string(this->GetPoint());
		case PokerColor::Heart:
			return std::string("红桃") + std::to_string(this->GetPoint());
		case PokerColor::Spade:
			return std::string("黑桃") + std::to_string(this->GetPoint());
		case PokerColor::Joker:
			if (this->GetPoint() == 14)
			{
				return std::string("joker");
			}
			else
			{
				return std::string("Joker");
			}
		}
	}

	std::string PokerCard::ToString(bool styled) const
	{
		if (!styled)
		{
			return this->ToString();
		}
		else
		{
			PokerPoint point = this->GetPoint();
			if (point == 15)
			{
				return std::string("JOKER");
			}
			if (point == 14)
			{
				return std::string("Joker");
			}
			PokerColor color = this->GetColor();
			std::string colorChar("");
			std::string pointChar("");
			switch (point)
			{
			case 13:
				pointChar = std::string("K");
				break;
			case 12:
				pointChar = std::string("Q");
				break;
			case 11:
				pointChar = std::string("J");
				break;
			case 1:
				pointChar = std::string("A");
				break;
			default:
				pointChar = std::to_string(point);
			}
			switch (color)
			{
		
			case PokerGame::PokerColor::Empty:
				break;
			case PokerGame::PokerColor::Spade:
				colorChar = std::string("♠");
				break;
			case PokerGame::PokerColor::Heart:
				colorChar = std::string("♥");
				break;
			case PokerGame::PokerColor::Club:
				colorChar = std::string("♣");
				break;
			case PokerGame::PokerColor::Diamond:
				colorChar = std::string("♦");
				break;
			case PokerGame::PokerColor::Joker:
				break;
			default:
				break;
			}
			return colorChar + pointChar;
		}
	}

	PokerColor PokerCard::GetColor() const
	{
		if (this->id == 53 || this->id == 54)
		{
			return PokerColor::Joker;
		}
		else
		{
			int color_num = (this->id - 1) / 13 + 1;
			return static_cast<PokerColor>(color_num);
		}
	}
#pragma endregion


#pragma region PokerCardCollection
	CardRepetitionDict PokerCardCollection::GetCardRepetition(PokerCardCollection& collection)
	{
		CardRepetitionDict repetition;
		for (int i = 0; i < collection.Count(); i++)
		{
			PokerCard card = collection[i];
			PokerPoint point = card.GetPoint();
			repetition[point] += 1;
		}
		return repetition;
	}
#pragma endregion


#pragma region 基于ID的CardCollection
	int IdBasedCardCollection::Count()
	{
		return this->cards.size();
	}

	PokerCard IdBasedCardCollection::PickOut(int index)
	{
		PokerCard card(this->cards[index]);
		this->cards.erase(this->cards.begin() + index);
		return card;
	}

	PokerCardCollection& IdBasedCardCollection::PickOut(PokerCardCollection& picked)
	{
		for (int i = 0; i < picked.Count(); i++)
		{
			PokerCardId id = picked[i].GetId();
			auto pos = std::find(this->cards.begin(), this->cards.end(), id);
			if (pos != this->cards.end())
			{
				this->cards.erase(pos);
			}
		}
		return *this;
	}

	PokerCardCollection& IdBasedCardCollection::operator<<(PokerCardId cardId)
	{
		this->cards.push_back(cardId);
		return *this;
	}

	PokerCardCollection& IdBasedCardCollection::operator<<(PokerCard card)
	{
		this->cards.push_back(card.GetId());
		return *this;
	}

	PokerCardCollection& IdBasedCardCollection::operator<<(PokerCardCollection& collection)
	{
		for (int i = 0; i < collection.Count(); i++)
		{
			*this << collection[i];
		}
		return *this;
	}

	PokerCard IdBasedCardCollection::operator[](int index)
	{
		try
		{
			PokerCard card(this->cards.at(index));
			return card;
		} 
		catch(std::out_of_range)
		{
			std::string err = std::string("index = ") + std::to_string(index);
			throw std::out_of_range(err);
		}	
	}

	bool IdBasedCardCollection::ContainsCard(PokerCard card)
	{
		throw NotImplementedException();
		return false;
	}

	bool IdBasedCardCollection::ContainsCollection(PokerCardCollection& other)
	{
		throw NotImplementedException();
		return false;
	}

	std::string IdBasedCardCollection::ToString()
	{
		std::string cardsStr("[");
		for (int i = 0; i < this->Count(); i++)
		{
			cardsStr += (*this)[i].ToString(true);
			cardsStr += std::string(",\t");
		}
		cardsStr += std::string("]");
		return cardsStr;
	}

	IdBasedCardCollection::~IdBasedCardCollection()
	{

	}

	IdBasedCardCollection::IdBasedCardCollection()
	{

	}

	IdBasedCardCollection::IdBasedCardCollection(int reserved)
	{
		this->cards.reserve(reserved);
	}
#pragma endregion


#pragma region 只能拿取的牌堆
	PokerCardCollection& TakeOnlyCardCollection::operator<<(PokerCardId cardId)
	{
		throw NotSupportedException();
	}

	PokerCardCollection& TakeOnlyCardCollection::operator<<(PokerCard card)
	{
		throw NotSupportedException();
	}

	PokerCardCollection& TakeOnlyCardCollection::operator<<(PokerCardCollection& collection)
	{
		throw NotSupportedException();
	}

	PokerCard TakeOnlyCardCollection::Pop()
	{
		PokerCardId id = this->cards.back();
		this->cards.pop_back();
		return PokerCard(id);
	}

	IdBasedCardCollection TakeOnlyCardCollection::Pop(int num)
	{
		IdBasedCardCollection collection;
		for (int i = 0; i < num; i++)
		{
			PokerCard card(this->cards.back());
			collection << card;
			this->cards.pop_back();
		}
		return collection;
	}

	TakeOnlyCardCollection& TakeOnlyCardCollection::operator>>(PokerCardCollection& other)
	{
		PokerCard card = this->Pop();
		other << card;
		return *this;
	}

	TakeOnlyCardCollection TakeOnlyCardCollection::Standard54(bool sorted)
	{
		TakeOnlyCardCollection collection;
		for (int i = 1; i <= 54; i++)
		{
			collection.cards.push_back(i);
		}
		if (!sorted)
		{
			collection.Shuffle();
		}
		return collection;
	}

	void TakeOnlyCardCollection::Shuffle()
	{
		//std::random_shuffle(this->cards.begin(), this->cards.end());
		std::default_random_engine g(time(0));
		std::shuffle(this->cards.begin(), this->cards.end(), g);
	}
#pragma endregion


#pragma region 有序的牌堆
	bool PokerCardComparatorFALStyleLessThan::operator()(PokerCard& left, PokerCard& right) const
	{
		PokerPoint
			left_num = left.Get3BasedNum(),
			right_num = right.Get3BasedNum();
		if (left_num != right_num)
		{
			return left_num < right_num;
		}
		else
		{
			int
				left_color = static_cast<int>(left.GetColor()),
				right_color = static_cast<int>(right.GetColor());
			return (left_color < right_color);
		}
	}
	bool PokerCardComparatorFALStyleLessThan::operator()(PokerCardId& leftId, PokerCardId& rightId) const
	{
		PokerCard left(leftId), right(rightId);
		return this->operator()(left, right);
	}

	//PokerCardCollection& SortedCardCollection::PickOut(PokerCardCollection& picked)
	//{
	//	throw NotImplementedException();
	//}

	PokerCardCollection& SortedCardCollection::operator<<(PokerCardId cardId)
	{
		this->cards.push_back(cardId);
		this->Sort();
		return *this;
	}

	PokerCardCollection& SortedCardCollection::operator<<(PokerCard card)
	{
		(*this) << card.GetId();
		return *this;
	}

	PokerCardCollection& SortedCardCollection::operator<<(PokerCardCollection& collection)
	{
		for (int i = 0; i < collection.Count(); i++)
		{
			(*this) << collection[i].GetId();
		}
		return *this;
	}

	void SortedCardCollection::Sort()
	{
		std::sort(this->cards.begin(), this->cards.end(), FAL_IsSmallerCard());
	}
#pragma endregion



}