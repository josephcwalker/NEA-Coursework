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
		GameOverState(Connect4Account *player1, Connect4Account *player2, SavedGame previousGame)
			: m_BackButton(onClickFunction(BackButtonFunction), this),
			  m_RematchButton(onClickFunction(RematchButtonFunction), this),
			  m_Player1(player1), m_Player2(player2), m_PreviousGame(previousGame)
		{
			LOG_TRACE("Game Over State created");
		}

		virtual ~GameOverState() { LOG_TRACE("Game Over State Deleted"); }

	public:

		void Initialize()
		{
			TrainAI();
		}

		void Execute()
		{
			m_RematchButton.OnMouseUpdate(m_Window);
		}

		void Draw()
		{
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

	private:
		Text m_Title;
		Text m_GameInfo;
		Text m_TrainingMessage;

		Button m_BackButton;
		Button m_RematchButton;
	};
}