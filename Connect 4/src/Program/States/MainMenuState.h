#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Title.h"

#include "AccountsState.h"

namespace Connect
{
	class MainMenuState : public State
	{
	public:
		MainMenuState()
			: m_ExitButton(State::PushConfirmExitState, this),
			  m_PlayButton(onClickFunction(PlayButtonFunction), this),
			  m_AccountsButton(onClickFunction(AccountsButtonFunction), this),
			  m_LeaderboardButton(onClickFunction(LeaderboardButtonFunction), this)
		{
			LOG_TRACE("Created Main Menu State");
		}

		virtual ~MainMenuState() { LOG_TRACE("Deleting Main Menu State"); }

	public:
		void Initialize() override
		{
			m_ExitButton.SetPosition(sf::Vector2f(1280.0f - 150.0f, 720.0f - 150.0f));
			m_ExitButton.SetSize(sf::Vector2f(125.0f, 125.0f));
			m_ExitButton.SetText("Exit");

			m_PlayButton.SetPosition(sf::Vector2f(440.0f, 300.0f));
			m_PlayButton.SetSize(sf::Vector2f(400.0f, 75.0f));
			m_PlayButton.SetText("Play");

			m_AccountsButton.SetPosition(sf::Vector2f(440.0f, 400.0f));
			m_AccountsButton.SetSize(sf::Vector2f(400.0f, 75.0f));
			m_AccountsButton.SetText("Accounts");

			m_LeaderboardButton.SetPosition(sf::Vector2f(440.0f, 500.0f));
			m_LeaderboardButton.SetSize(sf::Vector2f(400.0f, 75.0f));
			m_LeaderboardButton.SetText("Leaderboard");

			m_Title.SetText("  Connect 4 with\nMachine Learning");
			m_Title.SetCharacterSize(48);
			m_Title.SetPosition(sf::Vector2f(470.0f, 50.0f));
		}

		void Execute() override
		{
			m_ExitButton.OnMouseUpdate(m_Window);
			m_PlayButton.OnMouseUpdate(m_Window);
			m_AccountsButton.OnMouseUpdate(m_Window);
			m_LeaderboardButton.OnMouseUpdate(m_Window);
		}

		void Draw() override
		{
			m_Window->draw(m_ExitButton);
			m_Window->draw(m_PlayButton);
			m_Window->draw(m_AccountsButton);
			m_Window->draw(m_LeaderboardButton);

			m_Window->draw(m_Title);
		}

	// Button Functions
	private:
		void ExitButtonFunction() { LOG_TRACE("Exit Button Pressed"); Program::s_Instance->PopState(); }
		void PlayButtonFunction() { LOG_TRACE("Play Button Pressed"); Program::s_Instance->PopState(); }
		void AccountsButtonFunction() { LOG_TRACE("Accounts Button Pressed"); Program::s_Instance->PushState(new AccountsState()); }
		void LeaderboardButtonFunction() { LOG_TRACE("Leaderboard Button Pressed"); Program::s_Instance->PopState(); }

	private:
		Button m_ExitButton;
		Button m_PlayButton;
		Button m_AccountsButton;
		Button m_LeaderboardButton;

		Title m_Title;
	};
}