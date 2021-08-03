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

		class InvalidTypeException : public std::exception
		{

		};

		class NotSameTypeException : public InvalidTypeException
		{

		};

		class TypedCardCollection : public IdBasedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType() = 0;
			virtual bool IsSameType(TypedCardCollection& other) = 0;
			virtual bool IsLargerThan(TypedCardCollection& other) = 0;
			/// <summary>
			/// 尝试通过一个PokerCardCollection构造一个与此TypedCardCollection相同类型的对象,
			/// 如果PokerCardCollection不是匹配的牌型,会抛出NotSameTypeException
			/// </summary>
			/// <param name="collection"></param>
			/// <returns>如果是相同的牌型，会返回一个指向抽象基类的智能指针，对象的实际类型与自身相同</returns>
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection) = 0;
			virtual ~TypedCardCollection() {}
			static TypedCardCollection& TryCast(PokerCardCollection& collection);
			static TypedCardCollection& TryCastZhaDanOnly(PokerCardCollection& collection);
		};

		class DanZhangCollection : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection);
			DanZhangCollection(PokerCard card);
			DanZhangCollection(PokerCardId id);
			DanZhangCollection(PokerCardCollection& collection);
		private:

		};

		class DuiZiColleciton : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection);
			DuiZiColleciton(PokerCardCollection& collection);
		private:
			PokerPoint Get3BasedNum();
		};

		class SanZhangCollection : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection);
			SanZhangCollection(PokerCardCollection& collection);
		private:
			PokerPoint mainPoint;
			int attachedCount;
			PokerPoint attachedPoint;
		};

		class ShunZiCollection : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection);
			ShunZiCollection(PokerCardCollection& collection);
		private:
			PokerPoint upper3BasedPoint;
			PokerPoint lower3BasedPoint;
		};

		class ZhaDanCollection : public TypedCardCollection
		{
		public:
			virtual GeneralCardType GetGeneralType();
			virtual bool IsSameType(TypedCardCollection& other);
			virtual bool IsLargerThan(TypedCardCollection& other);
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection);
			ZhaDanCollection(PokerCardCollection& collection);
		private:
			PokerPoint threeBasedNum;
		};
	}
}