#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "ConnectGameState.h"

#include "Program/Connect4Account.h"
#include "Program/Systems/Connect4.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"
#include "Graphics/TextInput.h"

namespace Connect
{
	class GameOverState : public State
	{
	public:
		GameOverState(Connect4Account* player1, Connect4Account* player2, SavedGame previousGame, Outcome results)
			: m_BackButton(onClickFunction(BackButtonFunction), this),
			  m_RematchButton(onClickFunction(RematchButtonFunction), this),
			  m_Player1(player1), m_Player2(player2), m_PreviousGame(previousGame), m_GameOutcome(results)
		{
			LOG_TRACE("Game Over State created");
		}

		virtual ~GameOverState() { LOG_TRACE("Game Over State Deleted"); }

	public:
		void Initialize()
		{
			TrainAI();

			m_BackButton.SetPosition(sf::Vector2f(1280.0f - 150.0f, 720.0f - 150.0f));
			m_BackButton.SetSize(sf::Vector2f(125.0f, 125.0f));
			m_BackButton.SetText("Back");

			m_RematchButton.SetPosition(sf::Vector2f(1280.0f / 2.0f - 100.0f, 720.0f - 175.0f));
			m_RematchButton.SetSize(sf::Vector2f(200.0f, 125.0f));
			m_RematchButton.SetText("Rematch");

			m_Title.SetPosition(sf::Vector2f(50.0f, 50.0f));
			m_Title.SetCharacterSize(48);
			if (m_GameOutcome == Outcome::DRAW)
				m_Title.SetText("Game Drawn");
			else if (m_GameOutcome == Outcome::PLAYER1WIN)
				m_Title.SetText("Congratulations\n" + m_Player1->GetName());
			else
				m_Title.SetText("Congratulations\n" + m_Player2->GetName());

			m_GameInfo.SetPosition(sf::Vector2f(350.0f, 200.0f));
			m_GameInfo.SetCharacterSize(36);
			m_GameInfo.SetText("Any human player's AIs have been trained\n       on this and previous games");
		}

		void Execute()
		{
			m_BackButton.OnMouseUpdate(m_Window);
			m_RematchButton.OnMouseUpdate(m_Window);
		}

		void Draw()
		{
			m_Window->draw(m_Title);
			m_Window->draw(m_GameInfo);

			m_Window->draw(m_BackButton);
			m_Window->draw(m_RematchButton);
		}

	// Button Functions
	private:
		void BackButtonFunction();
		void RematchButtonFunction();

	private:
		void TrainAI();

	private:
		Connect4Account* m_Player1;
		Connect4Account* m_Player2;

		SavedGame m_PreviousGame;

		Outcome m_GameOutcome;

	private:
		Text m_Title;
		Text m_GameInfo;

		Button m_BackButton;
		Button m_RematchButton;
	};
}