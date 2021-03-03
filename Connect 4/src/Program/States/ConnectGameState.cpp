#include "ConnectGameState.h"

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
	}

	void ConnectGameState::ColumnChoiceButtonFunction()
	{
		// Make sure that it is a humans turn
		if (m_Game.isPlayer1Turn() && m_IsPlayer1AI)
			return;
		if (!m_Game.isPlayer1Turn() && m_IsPlayer2AI)
			return;

		LOG_TRACE("Column choice button pressed");
		if (m_Game.isValidPosition(recentButton->id))
		{
			m_Game.PlaceCounter(recentButton->id);
			m_SavedGame.columnChoices.push_back(recentButton->id);
		}

		for (int i : m_SavedGame.columnChoices)
			printf("%i, ", i);
	}
}