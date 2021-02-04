#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Title.h"

namespace Connect
{
	class ConfirmExitState : public State
	{
	public:
		ConfirmExitState()
			: m_ExitProgramButton(State::RemoveAllStates, this),
			  m_CancelButton(State::PopState, this)
		{
			LOG_TRACE("Created Confirm Exit State");
		}
		virtual ~ConfirmExitState() { LOG_TRACE("Deleting Confirm Exit State"); }

	public:
		void Initialize() override
		{
			m_ExitProgramButton.SetPosition(sf::Vector2f(300.0f, 340.0f));
			m_ExitProgramButton.SetSize(sf::Vector2f(220.0f, 100.0f));
			m_ExitProgramButton.SetText("Exit Program");

			m_CancelButton.SetPosition(sf::Vector2f(760.0f, 340.0f));
			m_CancelButton.SetSize(sf::Vector2f(220.0f, 100.0f));
			m_CancelButton.SetText("Cancel");

			m_Title.SetText(" Are You Sure You\nWould Like to Exit\n    the Program?");
			m_Title.SetCharacterSize(48);
			m_Title.SetPosition(sf::Vector2f(470.0f, 50.0f));
		}

		void Execute() override
		{
			m_ExitProgramButton.OnMouseUpdate(m_Window);
			m_CancelButton.OnMouseUpdate(m_Window);
		}

		void Draw() override
		{
			m_Window->draw(m_ExitProgramButton);
			m_Window->draw(m_CancelButton);
			m_Window->draw(m_Title);
		}

	private:
		Button m_ExitProgramButton;
		Button m_CancelButton;

		Title m_Title;
	};
}