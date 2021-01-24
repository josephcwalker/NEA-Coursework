#pragma once

#include "Tools/Logger.h"
#include "../State.h"

#include <SFML/Graphics.hpp>

namespace Connect
{
	class MainMenuState : public State
	{
	public:
		MainMenuState()
		{
			m_Test = sf::CircleShape(50.0f);
		}
		~MainMenuState() {}

	public:
		void Initialize() override
		{
			m_Test.setFillColor(sf::Color(0xFFFFFFFF));
		}

		void Execute() override
		{

		}

		void Draw() override
		{
			m_Window->draw(m_Test);
		}

	private:
		sf::CircleShape m_Test;
	};
}