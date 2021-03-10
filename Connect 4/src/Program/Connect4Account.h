#pragma once

#include "Tools/Logger.h"
#include "Account.h"

#include "Program/Systems/Connect4.h"

#include <fstream>

namespace Connect
{
	class Connect4Account : public Account
	{
	public:
		Connect4Account(std::string username)
			: Account("saves/" + username + "/accountData.bin")
		{
			// Make sure that the Neural network is loaded for playing the game/training
			LoadNeuralNetwork();
		}

		virtual ~Connect4Account() {}

	public:
		virtual int ColumnChoice(Connect4& board, bool player1) { return -1; };

		virtual void TrainOnGames()
		{
			LOG_TRACE("Training on previous games");

			std::vector<TrainingExample> gamesToTrainOn;

			// Load games from file
			std::fstream file("saves/" + GetName() + "/previousGames.gam");
			if (!file.is_open())
			{
				LOG_ERROR("Cannot open previous games file");
				return;
			}

			std::vector<std::pair<SavedGame, bool>> previousGames;

			// Read all games in the file
			while (!file.eof())
			{
				std::string str;
				file >> str;

				// Navigate to start of game
				if (str != "{")
					continue;

				previousGames.push_back(std::pair<SavedGame, bool>());

				// Read bool for is that game was played as player 1
				file >> str;
				previousGames.back().second = std::stoi(str);

				// Read in all moves until the end
				file >> str;
				while (str != "}")
				{
					previousGames.back().first.columnChoices.push_back(std::stoi(str));
					file >> str;
				}
			}

			// Convert all games into correct format for neural network
			for (std::pair<SavedGame, bool> p : previousGames)
			{
				int firstMove = p.second ? 0 : 1;

				for (int i = firstMove; i < p.first.columnChoices.size() - 2; i += 2)
				{
					Connect4 simulatedGame = SimulateGame(p.first, i);
					auto input = ConvertBoard(simulatedGame, p.second);
					auto output = ConvertOutput(p.first.columnChoices[i + 2]);

					gamesToTrainOn.push_back({ input, output });
				}
			}

			previousGames.clear();

			// Train on games multiple times
			int repeats = 1000 / gamesToTrainOn.size();

			for (int i = 0; i < repeats; i++)
				m_NeuralNetwork->Train(gamesToTrainOn, 1.0f);

			// Save updated Neural Network
			SaveToFile();
		}

		virtual void SaveGame(SavedGame& previousGame, bool player1)
		{
			// Open file
			std::ofstream file("saves/" + GetName() + "/previousGames.gam", std::ios::app);

			if (!file.is_open())
			{
				LOG_ERROR("Unable to open previous games file");
				return;
			}

			// Write new game to file
			file << "{ ";
			file << player1 << ' ';
			for (int i : previousGame.columnChoices)
				file << i << ' ';
			file << "}\n";

			file.close();
		}

	protected:
		// Convert board to Neural Network input format
		Eigen::VectorXf ConvertBoard(Connect4& board, bool player1)
		{
			auto boardArray = board.GetBoard();

			// Make sure -1 corresponds to the right player
			int multiplier = player1 ? 1 : -1;

			Eigen::VectorXf output(BOARD_WIDTH * BOARD_HEIGHT, 1);
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				for (int j = 0; j < BOARD_HEIGHT; j++)
					output[j * BOARD_WIDTH + i] = (float)boardArray[i][j] * multiplier;
			}

			return output;
		}

		// Convert column choice to Neural Network output
		Eigen::VectorXf ConvertOutput(int columnChoice)
		{
			Eigen::VectorXf output = Eigen::VectorXf::Zero(BOARD_WIDTH);
			output[columnChoice] = 1.0f;

			return output;
		}

		// Run game and return list of moves up to a certain number
		Connect4 SimulateGame(SavedGame game, int moveNumber)
		{
			Connect4 simulation;
			for (int i = 0; i < moveNumber; i++)
				simulation.PlaceCounter(game.columnChoices[i]);

			return simulation;
		}
	};
}