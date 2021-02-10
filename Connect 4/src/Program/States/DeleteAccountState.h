#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"
#include "Graphics/TextInput.h"

namespace Connect
{
	class DeleteAccountState : public State
	{
	public:
		DeleteAccountState()
			: m_BackButton(State::PopState, this),
			  m_DeleteAccountButton(onClickFunction(DeleteAccountButtonFunction), this),
			  m_UsernameInput(this, false),
			  m_PasswordInput(this, true)
		{
			LOG_TRACE("Created Delete Account State");
		}
		virtual ~DeleteAccountState() { LOG_TRACE("Deleting Delete Account State"); }

	public:
		void Initialize() override
		{
			m_BackButton.SetPosition(sf::Vector2f(1130.0f, 570.0f));
			m_BackButton.SetSize(sf::Vector2f(125.0f, 125.0f));
			m_BackButton.SetText("Back");

			m_DeleteAccountButton.SetPosition(sf::Vector2f(440.0f, 540.0f));
			m_DeleteAccountButton.SetSize(sf::Vector2f(400.0f, 125.0f));
			m_DeleteAccountButton.SetText("Delete Account");

			m_UsernameInput.SetPosition(sf::Vector2f(400.0f, 150.0f));
			m_UsernameInput.SetSize(sf::Vector2f(480.0f, 60.0f));
			m_UsernameInput.SetText("Username: ");

			m_PasswordInput.SetPosition(sf::Vector2f(400.0f, 240.0f));
			m_PasswordInput.SetSize(sf::Vector2f(480.0f, 60.0f));
			m_PasswordInput.SetText("Password: ");

			m_Title.SetPosition(sf::Vector2f(460.0f, 50.0f));
			m_Title.SetCharacterSize(48);
			m_Title.SetText("Delete an Account");

			m_Messages.SetPosition(sf::Vector2f(50.0f, 500.0f));
			m_Messages.SetCharacterSize(18);
			m_Messages.SetText("");
		}

		void Execute() override
		{
			m_BackButton.OnMouseUpdate(m_Window);
			m_DeleteAccountButton.OnMouseUpdate(m_Window);

			m_UsernameInput.OnMouseUpdate(m_Window);
			m_UsernameInput.KeyboardEvent();

			m_PasswordInput.OnMouseUpdate(m_Window);
			m_PasswordInput.KeyboardEvent();
		}

		void Draw() override
		{
			m_Window->draw(m_BackButton);
			m_Window->draw(m_DeleteAccountButton);

			m_Window->draw(m_UsernameInput);
			m_Window->draw(m_PasswordInput);

			m_Window->draw(m_Title);
			m_Window->draw(m_Messages);
		}

		// Button Functions
	private:
		void DeleteAccountButtonFunction();

		std::string CheckInputsAreValid();

	private:
		Button m_BackButton;
		Button m_DeleteAccountButton;

		TextInput m_UsernameInput;
		TextInput m_PasswordInput;

		Text m_Title;
		Text m_Messages;
	};
}