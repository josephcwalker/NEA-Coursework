#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Title.h"

#include <SFML/Graphics.hpp>

namespace Connect
{
	class MainMenuState : public State
	{
	public:
		MainMenuState() {}
		~MainMenuState() {}

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

		}

		void Draw() override
		{
			m_Window->draw(m_ExitButton);
			m_Window->draw(m_PlayButton);
			m_Window->draw(m_AccountsButton);
			m_Window->draw(m_LeaderboardButton);

			m_Window->draw(m_Title);
		}

	private:
		Button m_ExitButton;
		Button m_PlayButton;
		Button m_AccountsButton;
		Button m_LeaderboardButton;

		Title m_Title;
	};
}