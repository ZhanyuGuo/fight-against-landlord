#pragma once
#include <string>
#include <vector>

namespace PokerGame
{
	using PokerCardId = int;
	using PokerPoint = int;
	enum class PokerColor {
		Empty, Spade, Heart, Club, Diamond, Joker
	};

	class NotImplementedException : public std::exception
	{
	public:

	};

	class NotSupportedException : public std::exception
	{
	public:

	};

	class PokerCard 
	{
	public:
		PokerCard();
		PokerCard(PokerColor color, PokerPoint point);
		PokerCard(PokerCardId id);
		virtual PokerCardId GetId() const;
		virtual PokerColor GetColor() const;
		virtual PokerPoint GetPoint() const;
		virtual PokerPoint Get3BasedNum() const;
		static PokerPoint NormalPointTo3BasedNum(PokerPoint point);
		virtual std::string ToString() const;
		virtual std::string ToString(bool styled) const;
	private:
		PokerCardId id;
	};

	class PokerCardCollection
	{
	public:
		virtual int Count() = 0;
		virtual PokerCard PickOut(int index) = 0;
		virtual PokerCardCollection& PickOut(PokerCardCollection& picked) = 0;
		virtual PokerCardCollection& operator<< (PokerCardId cardId) = 0;
		virtual PokerCardCollection& operator<< (PokerCard card) = 0;
		virtual PokerCardCollection& operator<< (PokerCardCollection& collection) = 0;
		virtual PokerCard operator[](int index) = 0;
		//virtual const PokerCard operator[](int index) const = 0;
		virtual bool ContainsCard(PokerCard card) = 0;
		virtual bool ContainsCollection(PokerCardCollection& other) = 0;
		virtual std::string ToString() = 0;
		virtual ~PokerCardCollection() {}
		
	};

	class IdBasedCardCollection : public PokerCardCollection
	{
	public:
		virtual int Count();
		virtual PokerCard PickOut(int index);
		virtual PokerCardCollection& PickOut(PokerCardCollection& picked);
		virtual PokerCardCollection& operator<< (PokerCardId cardId);
		virtual PokerCardCollection& operator<< (PokerCard card);
		virtual PokerCardCollection& operator<< (PokerCardCollection& collection);
		virtual PokerCard operator[](int index);
		virtual bool ContainsCard(PokerCard card);
		virtual bool ContainsCollection(PokerCardCollection& other);
		virtual std::string ToString();
		virtual ~IdBasedCardCollection();
		IdBasedCardCollection();
		IdBasedCardCollection(int reserved);
		
	protected:
		std::vector<PokerCardId> cards;
	};

	class TakeOnlyCardCollection : public IdBasedCardCollection
	{
	public:
		virtual PokerCardCollection& operator<< (PokerCardId cardId);
		virtual PokerCardCollection& operator<< (PokerCard card);
		virtual PokerCardCollection& operator<< (PokerCardCollection& collection);
		virtual ~TakeOnlyCardCollection() {}
		PokerCard Pop();
		IdBasedCardCollection Pop(int num);
		TakeOnlyCardCollection& operator>> (PokerCardCollection& other);
		static TakeOnlyCardCollection Standard54(bool sorted=false);
	private:
		void Shuffle();
	};

	class SortedCardCollection : public IdBasedCardCollection
	{
	public:
		//using IdBasedCardCollection::PickOut;
		//virtual PokerCardCollection& PickOut(PokerCardCollection& picked);
		virtual PokerCardCollection& operator<< (PokerCardId cardId);
		virtual PokerCardCollection& operator<< (PokerCard card);
		virtual PokerCardCollection& operator<< (PokerCardCollection& collection);
		virtual ~SortedCardCollection() {}
	private:
		void Sort();
	};


}