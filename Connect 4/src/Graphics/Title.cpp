#include "Title.h"

#include "Tools/Logger.h"

namespace Connect
{
	
	Title::Title(sf::Vector2f position, std::string text)
	{
		LOG_TRACE("Loading font from file");
		if (!m_Font.loadFromFile("res/font/Calibri-Regular.ttf"))
		{
			LOG_ERROR("Error whilst loading font from file");
		}

		m_Text.setFont(m_Font);

		SetPosition(position);
		SetCharacterSize(24);
		SetText(text);

		LOG_TRACE("Title object created");
	}

	Title::~Title() {}

	void Title::SetPosition(sf::Vector2f position)
	{
		m_Text.setPosition(position);
	}

	void Title::SetCharacterSize(unsigned int size)
	{
		m_Text.setCharacterSize(size);
	}

	void Title::SetText(std::string text)
	{
		m_Text.setString(text);
	}

	void Title::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(m_Text);
	}
}