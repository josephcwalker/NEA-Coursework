#include "Text.h"

#include "Tools/Logger.h"
#include "Program/Program.h"

namespace Connect
{

	Text::Text(sf::Vector2f position, std::string text)
	{
		m_Text.setFont(*Program::s_Font);

		SetPosition(position);
		SetCharacterSize(24);
		SetText(text);

		LOG_TRACE("Title object created");
	}

	Text::~Text() {}

	void Text::SetPosition(sf::Vector2f position)
	{
		m_Text.setPosition(position);
	}

	void Text::SetCharacterSize(unsigned int size)
	{
		m_Text.setCharacterSize(size);
	}

	void Text::SetText(std::string text)
	{
		m_Text.setString(text);
	}

	void Text::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(m_Text);
	}
}