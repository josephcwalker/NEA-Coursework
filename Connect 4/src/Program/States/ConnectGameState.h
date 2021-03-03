#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Program/Connect4Account.h"
#include "Program/Systems/Connect4.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"
#include "Graphics/TextInput.h"

namespace Connect
{
	class ConnectGameState : public State
	{
	public:
		ConnectGameState(Connect4Account* player1, bool player1AI, Connect4Account* player2, bool player2AI)
		{
			// Create column choice buttons
			m_ColumnChoiceButtons.reserve(BOARD_WIDTH);
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				m_ColumnChoiceButtons.emplace_back(Button(onClickFunction(ColumnChoiceButtonFunction), this, std::to_string(i + 1)));

				float xcoord = 227.0f + i * 125.0f;
				m_ColumnChoiceButtons.back().SetPosition(sf::Vector2f(xcoord, 25.0f));
				m_ColumnChoiceButtons.back().SetSize(sf::Vector2f(75.0f, 75.0f));

				m_ColumnChoiceButtons.back().id = i;
			}

			// Save players and whether they are AI or not
			m_Player1 = player1;
			m_Player2 = player2;

			m_IsPlayer1AI = player1AI;
			m_IsPlayer2AI = player2AI;

			LOG_TRACE("Created Connect 4 Game state");
		}

		virtual ~ConnectGameState() { LOG_TRACE("Deleting Connect 4 Game state"); }

	public:
		void Initialize() override
		{
			m_Player1Info.SetPosition(sf::Vector2f(50.0f, 600.0f));
			m_Player1Info.SetCharacterSize(24);
			m_Player1Info.SetText("");

			m_Player2Info.SetPosition(sf::Vector2f(1100.0f, 600.0f));
			m_Player2Info.SetCharacterSize(24);
			m_Player2Info.SetText("");
		}

		void Execute() override
		{
			PlayAITurn();

			// Execute buttons if they have been pressed
			for (Button& b : m_ColumnChoiceButtons)
				b.OnMouseUpdate(m_Window);
		}

		void Draw() override
		{
			m_Window->draw(m_Player1Info);
			m_Window->draw(m_Player2Info);

			for (Button &b : m_ColumnChoiceButtons)
				m_Window->draw(b);
		}

	private:
		void PlayAITurn();

	// Button Functions
	private:
		void ColumnChoiceButtonFunction();

	private:
		Connect4Account* m_Player1;
		Connect4Account* m_Player2;

		bool m_IsPlayer1AI;
		bool m_IsPlayer2AI;

		unsigned int m_WaitTimer = 0;

		Connect4 m_Game;
		SavedGame m_SavedGame;

	private:
		Text m_Player1Info;
		Text m_Player2Info;

		std::vector<Button> m_ColumnChoiceButtons;
	};
}