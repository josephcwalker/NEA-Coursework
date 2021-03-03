#pragma once

#include "Tools/Logger.h"

#define BOARD_WIDTH 7
#define BOARD_HEIGHT 6

namespace Connect
{
	enum class BoardSpace : char
	{
		EMPTY = 0,
		PLAYER1 = 1,
		PLAYER2 = 2
	};

	class Connect4
	{
	public:
		Connect4();
		virtual ~Connect4();

	public:

		inline bool isValidPosition(int column);		
		void PlaceCounter(int column);

		inline bool GameOver() { return GameWon() || GameDrawn(); }
		bool GameWon();
		bool GameDrawn();

	private:
		// Current State of the board
		BoardSpace m_Board[BOARD_WIDTH][BOARD_HEIGHT];

		// Most recent positions to allow for optimizations
		int m_RecentColumn;
		int m_RecentRow;

		BoardSpace m_CurrentPlayer;
	};
}