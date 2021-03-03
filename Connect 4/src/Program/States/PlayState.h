#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"
#include "Graphics/TextInput.h"

namespace Connect
{
	class PlayState : public State
	{
	public:
		PlayState()
			: m_BackButton(State::PopState, this),
			  m_PlayGameButton(onClickFunction(PlayGameButtonFunction), this),
			  m_IsPlayer1AIButton(onClickFunction(AIButtonFunction), this),
			  m_Player1Username(this),
			  m_Player1Password(this, true),
			  m_IsPlayer2AIButton(onClickFunction(AIButtonFunction), this),
			  m_Player2Username(this),
			  m_Player2Password(this, true)

		{
			LOG_TRACE("Created Leaderboard State");
		}
		virtual ~PlayState() { LOG_TRACE("Deleting Leaderboard State"); }

	public:
		void Initialize() override
		{
			m_BackButton.SetPosition(sf::Vector2f(1280.0f - 150.0f, 720.0f - 150.0f));
			m_BackButton.SetSize(sf::Vector2f(125.0f, 125.0f));
			m_BackButton.SetText("Back");

			m_PlayGameButton.SetPosition(sf::Vector2f(440.0f, 540.0f));
			m_PlayGameButton.SetSize(sf::Vector2f(400.0f, 125.0f));
			m_PlayGameButton.SetText("Play Game");

			m_Title.SetPosition(sf::Vector2f(480.0f, 50.0f));
			m_Title.SetCharacterSize(48);
			m_Title.SetText("Play Connect 4");

			// Player 1

			m_Player1Text.SetPosition(sf::Vector2f(50.0f, 120.0f));
			m_Player1Text.SetCharacterSize(36);
			m_Player1Text.SetText("Player 1");

			m_IsPlayer1AIButton.SetPosition(sf::Vector2f(50.0f, 200.0f));
			m_IsPlayer1AIButton.SetSize(sf::Vector2f(320.0f, 60.0f));
			m_IsPlayer1AIButton.SetText("AI: no", true);
			m_IsPlayer1AIButton.id = 1;
			m_IsPlayer1AI = false;

			m_Player1Username.SetPosition(sf::Vector2f(50.0f, 300.0f));
			m_Player1Username.SetSize(sf::Vector2f(550.0f, 60.0f));
			m_Player1Username.SetText("Username: ");

			m_Player1Password.SetPosition(sf::Vector2f(50.0f, 400.0f));
			m_Player1Password.SetSize(sf::Vector2f(550.0f, 60.0f));
			m_Player1Password.SetText("Password: ");

			m_Player1ErrorMessages.SetPosition(sf::Vector2f(50.0f, 500.0f));
			m_Player1ErrorMessages.SetCharacterSize(18);
			m_Player1ErrorMessages.SetText("");

			// Player 2

			m_Player2Text.SetPosition(sf::Vector2f(1100.0f, 120.0f));
			m_Player2Text.SetCharacterSize(36);
			m_Player2Text.SetText("Player 2");

			m_IsPlayer2AIButton.SetPosition(sf::Vector2f(910.0f, 200.0f));
			m_IsPlayer2AIButton.SetSize(sf::Vector2f(320.0f, 60.0f));
			m_IsPlayer2AIButton.SetText("AI: no", true);
			m_IsPlayer2AIButton.id = 2;
			m_IsPlayer2AI = false;

			m_Player2Username.SetPosition(sf::Vector2f(680.0f, 300.0f));
			m_Player2Username.SetSize(sf::Vector2f(550.0f, 60.0f));
			m_Player2Username.SetText("Username: ");

			m_Player2Password.SetPosition(sf::Vector2f(680.0f, 400.0f));
			m_Player2Password.SetSize(sf::Vector2f(550.0f, 60.0f));
			m_Player2Password.SetText("Password: ");

			m_Player2ErrorMessages.SetPosition(sf::Vector2f(850.0f, 500.0f));
			m_Player2ErrorMessages.SetCharacterSize(18);
			m_Player2ErrorMessages.SetText("");
		}

		void Execute() override
		{
			m_BackButton.OnMouseUpdate(m_Window);
			m_PlayGameButton.OnMouseUpdate(m_Window);

			// Player 1

			m_IsPlayer1AIButton.OnMouseUpdate(m_Window);
			m_Player1Username.OnMouseUpdate(m_Window);
			m_Player1Username.KeyboardEvent();
			if (!m_IsPlayer1AI)
			{
				m_Player1Password.OnMouseUpdate(m_Window);
				m_Player1Password.KeyboardEvent();
			}

			// Player 2

			m_IsPlayer2AIButton.OnMouseUpdate(m_Window);
			m_Player2Username.OnMouseUpdate(m_Window);
			m_Player2Username.KeyboardEvent();
			if (!m_IsPlayer2AI)
			{
				m_Player2Password.OnMouseUpdate(m_Window);
				m_Player2Password.KeyboardEvent();
			}
		}

		void Draw() override
		{
			m_Window->draw(m_BackButton);
			m_Window->draw(m_PlayGameButton);

			m_Window->draw(m_Title);

			// Player 1

			m_Window->draw(m_Player1Text);
			m_Window->draw(m_IsPlayer1AIButton);
			m_Window->draw(m_Player1Username);
			if (!m_IsPlayer1AI) m_Window->draw(m_Player1Password);
			m_Window->draw(m_Player1ErrorMessages);
			
			// Player 2

			m_Window->draw(m_Player2Text);
			m_Window->draw(m_IsPlayer2AIButton);
			m_Window->draw(m_Player2Username);
			if (!m_IsPlayer2AI)
				m_Window->draw(m_Player2Password);
			m_Window->draw(m_Player2ErrorMessages);
		}

	// Button Functions
	private:
	void AIButtonFunction();
	void PlayGameButtonFunction();

	std::string CheckInputsAreValid(TextInput &username, TextInput &password, bool AI);

	private:
		Button m_BackButton;
		Button m_PlayGameButton;

		TextInput m_Player1Username;
		TextInput m_Player1Password;
		Button m_IsPlayer1AIButton;
		bool m_IsPlayer1AI;

		TextInput m_Player2Username;
		TextInput m_Player2Password;
		Button m_IsPlayer2AIButton;
		bool m_IsPlayer2AI;

		Text m_Title;

		Text m_Player1Text;
		Text m_Player1ErrorMessages;
		Text m_Player2Text;
		Text m_Player2ErrorMessages;
	};
}