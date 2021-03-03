#pragma once

#include "Tools/Logger.h"
#include "Account.h"

#include "Program/Systems/Connect4.h"

namespace Connect
{
	class Connect4Account : public Account
	{
	public:
		Connect4Account(std::string username)
			: Account(username)
		{
			LoadNeuralNetwork();
		}

		virtual ~Connect4Account() {}

	public:
		virtual int ColumnChoice(Connect4& board, bool player1) { return -1; };

	protected:
		Eigen::VectorXf ConvertBoard(Connect4& board, bool player1)
		{
			auto boardArray = board.GetBoard();

			Eigen::VectorXf output(BOARD_WIDTH * BOARD_HEIGHT, 1);
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				for (int j = 0; j < BOARD_HEIGHT; j++)
				{
					output[j * BOARD_WIDTH + i] = (float)boardArray[i][j] - 1.0f;
				}
			}

			return output;
		}
	};
}