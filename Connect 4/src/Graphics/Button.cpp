#include "Button.h"

#include <functional>

#include "Tools/Logger.h"
#include "Program/Program.h"

namespace Connect
{
	Button::Button(onClickFunction function, State *parentState, std::string text)
		: m_ClickFunction(function), m_ParentState(parentState)
	{
		m_Rect.setFillColor(sf::Color(0x5E1F8DFF));
		m_Rect.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_Rect.setSize(sf::Vector2f(100.0f, 100.0f));

		m_Text.setFont(*Program::s_Font);
		m_Text.setString(text);
		m_Text.setCharacterSize(24);
		m_Text.setFillColor(sf::Color(0xD9D9D9FF));

		LOG_TRACE("Button object created");

		CenterText();
	}

	Button::~Button()
	{

	}

	void Button::SetPosition(sf::Vector2f position)
	{
		m_Rect.setPosition(position);
		CenterText();
	}

	void Button::SetSize(sf::Vector2f size)
	{
		m_Rect.setSize(size);
		CenterText();
	}

	void Button::SetText(std::string text)
	{
		m_Text.setString(text);
		CenterText();
	}

	void Button::OnMouseUpdate(sf::RenderWindow* window)
	{
		// Reset to default colour
		m_Rect.setFillColor(sf::Color(0x5E1F8DFF));

		// Check mouse is inside rectangle
		bool mouseInside = m_Rect.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)));
		if (!mouseInside)
			return;

		// Mouse hover colour
		m_Rect.setFillColor(sf::Color(0x6E2F9DFF));

		// Change state depending on what the button should do
		if (Program::s_Instance->WasMousePressed())
		{
			// Call whatever function this button is linked to
			std::invoke(m_ClickFunction, *m_ParentState);
		}
	}

	void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(m_Rect, states);
		target.draw(m_Text, states);
	}

	void Button::CenterText()
	{
		// Center Text in rectangle due to position being top left corner of object
		sf::Vector2f textPosition = m_Rect.getPosition() + 0.5f * (m_Rect.getSize() - m_Text.getGlobalBounds().getSize());
		textPosition.y -= m_Text.getCharacterSize() >> 2;
		m_Text.setPosition(textPosition);
	}
}