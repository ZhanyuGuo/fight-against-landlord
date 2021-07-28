#pragma once
#include "poker.h"
#include <memory>
#include <string>

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
			virtual ~TypedCardCollection(){}
		};


		class DanZhangCollection : public TypedCardCollection
		{
		public:

		private:
			PokerCardId id;
		};

		class DuiZiColleciton : public TypedCardCollection
		{
		public:

		private:
			
		};

		class CardEvent
		{
		public:
			enum class MotionType {
				Active, Follow, Pass
			};
			MotionType GetType() const;
			PokerCardCollection* GetContent() const;
			CardEvent(MotionType type, PokerCardCollection* content);
		private:
			MotionType type;
			PokerCardCollection* content;
		};



		class Player
		{
		public:
			virtual int PrepareResponse(int leastPoint) = 0;
			virtual CardEvent CardResponse(CardEvent e) noexcept = 0;
			virtual PokerCardCollection& GetCards() = 0;
			virtual void Reset() = 0;
		};

		class GameProcess
		{
		public:
			static const int PlayerNum = 3;
			GameProcess(std::shared_ptr<Player>& p1,
				std::shared_ptr<Player>& p2,
				std::shared_ptr<Player>& p3);
			~GameProcess();
			void Run();
		private:
			std::shared_ptr<Player> players[PlayerNum];
		};


		class OnlinePlayer : public Player
		{
		public:
			OnlinePlayer();
			OnlinePlayer(int port);
		};

		class LocalPlayer : public Player
		{

		};

		class StupidLocalPlayerForDebugging : public LocalPlayer
		{
		public:
			virtual int PrepareResponse(int leastPoint);
			virtual CardEvent CardResponse(CardEvent e) noexcept;
			virtual PokerCardCollection& GetCards();
			virtual void Reset();
			StupidLocalPlayerForDebugging(std::string name);
		private:
			SortedCardCollection cards;
			std::string name;
		};




	}



}