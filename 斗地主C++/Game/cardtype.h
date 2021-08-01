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
			/// <summary>
			/// ����ͨ��һ��PokerCardCollection����һ�����TypedCardCollection��ͬ���͵Ķ���,
			/// ���PokerCardCollection����ƥ�������,���׳�NotSameTypeException
			/// </summary>
			/// <param name="collection"></param>
			/// <returns>�������ͬ�����ͣ��᷵��һ��ָ�������������ָ�룬�����ʵ��������������ͬ</returns>
			virtual std::shared_ptr<TypedCardCollection> FormatCollection(PokerCardCollection& collection) = 0;
			virtual ~TypedCardCollection() {}
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
	}
}