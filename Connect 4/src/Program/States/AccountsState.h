#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Text.h"

namespace Connect
{
	class AccountsState : public State
	{
	public:
		AccountsState()
			: m_BackButton(State::PopState, this),
			  m_CreateAccountButton(onClickFunction(CreateAccountButtonFunction), this),
			  m_DeleteAccountButton(onClickFunction(DeleteAccountButtonFunction), this)
		{
			LOG_TRACE("Created Accounts State");
		}
		virtual ~AccountsState() { LOG_TRACE("Deleting Accounts State"); }

	public:
		void Initialize() override
		{
			m_BackButton.SetPosition(sf::Vector2f(1280.0f - 150.0f, 720.0f - 150.0f));
			m_BackButton.SetSize(sf::Vector2f(125.0f, 125.0f));
			m_BackButton.SetText("Back");

			m_CreateAccountButton.SetPosition(sf::Vector2f(300.0f, 340.0f));
			m_CreateAccountButton.SetSize(sf::Vector2f(220.0f, 100.0f));
			m_CreateAccountButton.SetText("Create Account");

			m_DeleteAccountButton.SetPosition(sf::Vector2f(760.0f, 340.0f));
			m_DeleteAccountButton.SetSize(sf::Vector2f(220.0f, 100.0f));
			m_DeleteAccountButton.SetText("Delete Account");

			m_Title.SetPosition(sf::Vector2f(540.0f, 50.0f));
			m_Title.SetCharacterSize(48);
			m_Title.SetText("Accounts");

			m_SubTitle.SetPosition(sf::Vector2f(500.0f, 130.0f));
			m_SubTitle.SetCharacterSize(36);
			m_SubTitle.SetText("What Would You\n      Like To Do?");
		}

		void Execute() override
		{
			m_BackButton.OnMouseUpdate(m_Window);
			m_CreateAccountButton.OnMouseUpdate(m_Window);
			m_DeleteAccountButton.OnMouseUpdate(m_Window);
		}

		void Draw() override
		{
			m_Window->draw(m_BackButton);
			m_Window->draw(m_CreateAccountButton);
			m_Window->draw(m_DeleteAccountButton);

			m_Window->draw(m_Title);
			m_Window->draw(m_SubTitle);
		}

	// Button Functions
	private:
		void CreateAccountButtonFunction() { LOG_TRACE("Create Account Button Pressed"); Program::s_Instance->PopState(); }
		void DeleteAccountButtonFunction() { LOG_TRACE("Delete Account Button Pressed"); Program::s_Instance->PopState(); }

	private:
		Button m_BackButton;
		Button m_CreateAccountButton;
		Button m_DeleteAccountButton;

		Text m_Title;
		Text m_SubTitle;
	};
}