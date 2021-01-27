#include "Button.h"

#include <algorithm>

#include "Tools/Logger.h"

namespace Connect
{
	Button::Button(sf::Vector2f position, sf::Vector2f size, std::string text)
	{
		LOG_TRACE("Loading font from file");
		if (!m_Font.loadFromFile("Connect 4/res/font/Calibri-Regular.ttf"))
		{
			LOG_ERROR("Error whilst loading font from file");
		}

		m_Rect.setFillColor(sf::Color(0x5E1F8DFF));
		m_Rect.setPosition(position);
		m_Rect.setSize(size);

		m_Text.setFont(m_Font);
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