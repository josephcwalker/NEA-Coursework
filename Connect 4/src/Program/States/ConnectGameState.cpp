#include "ConnectGameState.h"

#include "Program/Program.h"
#include "GameOverState.h"

namespace Connect
{
	void ConnectGameState::PlayAITurn()
	{
		// Only get column choice if it is an AI player's turn
		if (m_Game.isPlayer1Turn() && m_IsPlayer1AI)
		{
			// Allow time for the user to look at the board
			if (m_WaitTimer < 60)
			{
				m_WaitTimer++;
			}
			else
			{
				m_WaitTimer = 0;
				int columnChoice = m_Player1->ColumnChoice(m_Game, true);

				m_Game.PlaceCounter(columnChoice);
				m_SavedGame.columnChoices.push_back(columnChoice);
			}
		}

		// Repeat for player 2
		if (!m_Game.isPlayer1Turn() && m_IsPlayer2AI)
		{
			if (m_WaitTimer < 60)
			{
				m_WaitTimer++;
			}
			else
			{
				m_WaitTimer = 0;
				int columnChoice = m_Player2->ColumnChoice(m_Game, false);

				m_Game.PlaceCounter(columnChoice);
				m_SavedGame.columnChoices.push_back(columnChoice);
			}
		}

		// Check if game has finished
		if (m_Game.GameOver())
		{
			m_GameOver = true;
		}
	}

	void ConnectGameState::DrawCounters()
	{
		// Draw counters in the correct positions on the board
		auto board = m_Game.GetBoard();
		sf::CircleShape redCounter(45);
		sf::CircleShape yellowCounter(45);

		redCounter.setFillColor(sf::Color::Red);
		yellowCounter.setFillColor(sf::Color::Yellow);

		int leftSide = 245.0f;
		int topSide = 108.0f;

		// Loop through all positions and draw counters where they exist
		for (int i = 0; i < BOARD_WIDTH; i++)
		{
			for (int j = 0; j < BOARD_HEIGHT; j++)
			{
				if (board[i][j] == BoardSpace::PLAYER1)
				{
					redCounter.setPosition(leftSide + i * 116.0f, topSide + j * 103.0f);
					m_Window->draw(redCounter);
				}
				else if (board[i][j] == BoardSpace::PLAYER2)
				{
					yellowCounter.setPosition(leftSide + i * 116.0f, topSide + j * 103.0f);
					m_Window->draw(yellowCounter);
				}
			}
		}
	}

	void ConnectGameState::ColumnChoiceButtonFunction()
	{
		LOG_TRACE("Column choice button pressed");

		// Make sure that it is a humans turn
		if (m_Game.isPlayer1Turn() && m_IsPlayer1AI)
			return;
		if (!m_Game.isPlayer1Turn() && m_IsPlayer2AI)
			return;

		if (m_Game.isValidPosition(recentButton->id))
		{
			m_Game.PlaceCounter(recentButton->id);
			m_SavedGame.columnChoices.push_back(recentButton->id);
		}
	}

	void ConnectGameState::ContinueButtonFunction()
	{
		LOG_TRACE("Continue Button Pressed");
		Program::s_Instance->PushState(new GameOverState(m_Player1, m_Player2, m_SavedGame, m_Game.GetOutcome()));
		Program::s_Instance->PopState();
	}
}