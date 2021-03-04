#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Program/Connect4Account.h"
#include "Program/Systems/Connect4.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"
#include "Graphics/TextInput.h"

#include <SFML/Graphics.hpp>

namespace Connect
{
	class ConnectGameState : public State
	{
	public:
		ConnectGameState(Connect4Account *player1, bool player1AI, Connect4Account *player2, bool player2AI)
			: m_GameOver(false), m_WaitTimer(0),
			  m_ContinueButton(onClickFunction(ContinueButtonFunction), this)
		{
			// Create column choice buttons
			m_ColumnChoiceButtons.reserve(BOARD_WIDTH);
			for (int i = 0; i < BOARD_WIDTH; i++)
			{
				m_ColumnChoiceButtons.emplace_back(Button(onClickFunction(ColumnChoiceButtonFunction), this, std::to_string(i + 1)));

				float xcoord = 255.0f + i * 115.0f;
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
			m_ContinueButton.SetPosition(sf::Vector2f(1075.0f, 25.0f));
			m_ContinueButton.SetSize(sf::Vector2f(150.0f, 100.0f));
			m_ContinueButton.SetText("Continue");

			if (!m_BoardTexture.loadFromFile("res/images/Connect4Board.png"))
			{
				LOG_ERROR("Unable to load image from file");
				return;
			}

			m_BoardTexture.setSmooth(true);
			
			m_Board.setPosition(227.0f, 100.0f);
			m_Board.setScale(1.29f, 1.29f);
			m_Board.setTexture(m_BoardTexture);
		}

		void Execute() override
		{
			if (!m_GameOver)
			{
				PlayAITurn();

				// Execute buttons if they have been pressed
				for (Button& b : m_ColumnChoiceButtons)
					b.OnMouseUpdate(m_Window);

			}
			else
			{
				// Only use continue button when the game is over
				m_ContinueButton.OnMouseUpdate(m_Window);
			}
		}

		void Draw() override
		{
			m_Window->draw(m_Board);
			DrawCounters();

			if (m_GameOver)
				m_Window->draw(m_ContinueButton);
			else
				for (Button &b : m_ColumnChoiceButtons)
					m_Window->draw(b);
		}

	private:
		void PlayAITurn();

		void DrawCounters();

	// Button Functions
	private:
		void ColumnChoiceButtonFunction();
		void ContinueButtonFunction();

	private:
		Connect4Account* m_Player1;
		Connect4Account* m_Player2;

		bool m_IsPlayer1AI;
		bool m_IsPlayer2AI;

		unsigned int m_WaitTimer;
		bool m_GameOver;

		Connect4 m_Game;
		SavedGame m_SavedGame;

	private:
		std::vector<Button> m_ColumnChoiceButtons;

		Button m_ContinueButton;

		sf::Sprite m_Board;
		sf::Texture m_BoardTexture;
	};
}