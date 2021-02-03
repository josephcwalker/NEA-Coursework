#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include "Graphics/Button.h"
#include "Graphics/Title.h"

namespace Connect
{
	class AccountsState : public State
	{
	public:
		AccountsState()
			: m_BackButton(State::PopState, this)
		{}
		virtual ~AccountsState() {}

	public:
		void Initialize() override
		{

		}

		void Execute() override
		{
			m_BackButton.OnMouseUpdate(m_Window);
		}

		void Draw() override
		{
			m_Window->draw(m_BackButton);
		}

	// Button Functions
	private:


	private:
		Button m_BackButton;
	};
}