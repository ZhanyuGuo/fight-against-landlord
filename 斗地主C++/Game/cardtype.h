#pragma once
#include "poker.h"
#include <memory>

namespace PokerGame
{

	namespace FAL
	{
		enum class GeneralCardType
		{
			Dan,
			Dui,
			San,
			Zha,
			Shun,
			LianDui,
			FeiJi,
			LianZha,
			WangZha
		};

		class NotSameTypeException : public std::exception
		{

		};

		class TypedCardCollection : public IdBasedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType() = 0;
			virtual bool IsSameType(TypedCardCollection& other) = 0;
			virtual bool IsLargerThan(TypedCardCollection& other) = 0;
			virtual std::shared_ptr<TypedCardCollection> FromCollection(PokerCardCollection& collection) = 0;
			virtual ~TypedCardCollection() {}
		};


		class DanZhangCollection : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			virtual std::shared_ptr<TypedCardCollection> FromCollection(PokerCardCollection& collection);
			DanZhangCollection(PokerCard card);
			DanZhangCollection(PokerCardId id);
		private:

		};

		class DuiZiColleciton : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			DuiZiColleciton();
		private:

		};
	}
}