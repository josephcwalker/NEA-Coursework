#pragma once

#include "Tools/Logger.h"
#include "Connect4Account.h"

#include "Program/Systems/Connect4.h"

namespace Connect
{
	class Connect4AI : public Connect4Account
	{
	public:
		Connect4AI(std::string username)
			: Connect4Account(username)
		{}

	public:
		int ColumnChoice(Connect4& board, bool player1) override
		{
			LOG_TRACE("AI Made a choice");

			// Turn the board into a different type for the NN to use
			Eigen::VectorXf boardLayout = ConvertBoard(board, player1);

			auto choices = m_NeuralNetwork->FeedFoward(boardLayout);

			// Get highest ranking valid choice
			float max = 2.0f;
			int choice = -1;

			while (true)
			{
				float currentBest = 0.0f;

				for (int i = 0; i < BOARD_WIDTH; i++)
				{
					if (choices[i] < max)
					{
						if (choices[i] > currentBest)
						{
							currentBest = choices[i];
							choice = i;
						}
					}
				}

				// If the positions chosen is valid then use it
				// Otherwise find the next highest ranked one

				if (board.isValidPosition(choice))
					return choice;
				else
					max = currentBest;
			}
		}
	};
}