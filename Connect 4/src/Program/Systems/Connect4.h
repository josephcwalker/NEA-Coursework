#pragma once

#include "Tools/Logger.h"

#include <vector>
#include <cstring>

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

namespace Connect
{
	struct SavedGame
	{
		std::vector<int> columnChoices;
	};

	enum class BoardSpace : char
	{
		PLAYER1 = -1,
		EMPTY = 0,
		PLAYER2 = 1
	};

	class Connect4
	{
	public:
		Connect4();
		virtual ~Connect4();

	public:

		bool isValidPosition(int column);		
		void PlaceCounter(int column);

		inline bool GameOver() { return GameWon() || GameDrawn(); }
		bool GameWon();
		bool GameDrawn();

		inline const auto& GetBoard() { return m_Board; }

		inline bool isPlayer1Turn() { return m_CurrentPlayer == BoardSpace::PLAYER1; }

	private:
		// Current State of the board
		BoardSpace m_Board[BOARD_WIDTH][BOARD_HEIGHT];

		// Most recent positions to allow for optimizations
		int m_RecentColumn;
		int m_RecentRow;

		BoardSpace m_CurrentPlayer;
	};
}