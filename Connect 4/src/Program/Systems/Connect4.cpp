#include "Connect4.h"

namespace Connect
{
	Connect4::Connect4()
		: m_RecentColumn(-1), m_RecentRow(-1), m_CurrentPlayer(BoardSpace::PLAYER1)
	{
		// Initialize Board to empty
		memset(m_Board, 0, BOARD_HEIGHT * BOARD_WIDTH);
	}

	Connect4::~Connect4() {}

	inline bool Connect4::isValidPosition(int column)
	{
		// Check whether the chosen column is valid
		// If the top space in that column is empty then it is valid

		return m_Board[column][0] == BoardSpace::EMPTY;
	}

	void Connect4::PlaceCounter(int column)
	{
		int chosenRow = BOARD_HEIGHT - 1;

		// Places counter in chosen column
		// Loop downwards from top to simulate gravity
		// We can assume that the top space is open and can loop from i = 1
		for (int i = 1; i < BOARD_HEIGHT; i++)
		{
			// If its not empty then can chose the space above it
			if (m_Board[column][i] != BoardSpace::EMPTY)
			{
				chosenRow = i - 1;
				break;
			}
		}

		// Place counter
		m_Board[column][chosenRow] = m_CurrentPlayer;

		m_RecentColumn = column;
		m_RecentRow = chosenRow;

		// Swap players
		m_CurrentPlayer = (m_CurrentPlayer == BoardSpace::PLAYER1) ? BoardSpace::PLAYER2 : BoardSpace::PLAYER1;

		return;
	}

	bool Connect4::GameWon()
	{
		BoardSpace recentPlayer = (m_CurrentPlayer == BoardSpace::PLAYER1) ? BoardSpace::PLAYER2 : BoardSpace::PLAYER1;

		// Check in all possible lines (up-down, left-right, both diagonals)
		int dx[] = {0, -1, -1, -1};
		int dy[] = {-1, -1, 0, 1};

		// Loop through all directions
		for (int i = 0; i < 4; i++)
		{
			// The most recent one is already 1 in a row
			int inARow = 1;

			// Check all counters in a direction for 3 steps
			for (int direction = -1; direction < 2; direction += 2)
			{
				for (int step = 1; step < 4; step++)
				{
					// Position to check
					int x = m_RecentColumn + dx[i] * step * direction;
					int y = m_RecentRow + dy[i] * step * direction;

					// If out of bounds
					if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
						break;

					if (m_Board[x][y] != recentPlayer)
						break;

					inARow++;
				}
			}

			// If 4 or more counters were found then the game has been won
			if (inARow >= 4)
				return true;
		}
		return false;
	}

	bool Connect4::GameDrawn()
	{
		// If all spaces are filled then the game is drawn
		for (int i = 0; i < BOARD_WIDTH; i++)
			if (m_Board[i][0] == BoardSpace::EMPTY)
				return false;
		return true;
	}
}