#include "TextInput.h"

#include <functional>

#include "Tools/Logger.h"
#include "Program/Program.h"

namespace Connect
{
	TextInput::TextInput(State *parentState, bool passwordField, std::string text)
		: m_ParentState(parentState), m_PasswordInputField(passwordField), m_UserInput(""), m_DefaultText(text)
	{
		// Set default style and position
		m_Rect.setFillColor(sf::Color(0x5E1F8DFF));
		m_Rect.setPosition(sf::Vector2f(0.0f, 0.0f));
		m_Rect.setSize(sf::Vector2f(100.0f, 100.0f));

		m_Text.setFont(*Program::s_Font);
		m_Text.setString(m_DefaultText);
		m_Text.setCharacterSize(24);
		m_Text.setFillColor(sf::Color(0xD9D9D9FF));

		LOG_TRACE("Text Input Field object created");

		CenterTextLeft();
	}

	TextInput::~TextInput()
	{

	}

	void TextInput::SetPosition(sf::Vector2f position)
	{
		m_Rect.setPosition(position);
		CenterTextLeft();
	}

	void TextInput::SetSize(sf::Vector2f size)
	{
		m_Rect.setSize(size);
		CenterTextLeft();
	}

	void TextInput::SetText(std::string text)
	{
		// Default text cannot be deleted
		m_DefaultText = text;
		m_Text.setString(m_DefaultText);
		CenterTextLeft();
	}

	void TextInput::KeyboardEvent(void (State::*functionPointer)())
	{
		// Make sure the key pressed is available
		if (!m_Active || Program::s_KeyPressed == sf::Keyboard::Unknown)
			return;

		// Make sure that the key pressed was alphabetic
		if (Program::s_KeyPressed >= sf::Keyboard::A && Program::s_KeyPressed <= sf::Keyboard::Z)
			m_UserInput += 'a' + Program::s_KeyPressed;

		// If backspace was pressed removed last character
		if (Program::s_KeyPressed == sf::Keyboard::BackSpace && m_UserInput.size() > 0)
			m_UserInput.pop_back();

		// Display inputted text
		if (m_PasswordInputField)
			m_Text.setString(m_DefaultText + std::string(m_UserInput.size(), '*'));
		else
			m_Text.setString(m_DefaultText + m_UserInput);

		CenterTextLeft();

		// Call function whenever text has changed
		if (functionPointer)
			std::invoke(functionPointer, *m_ParentState);
	}

	void TextInput::OnMouseUpdate(sf::RenderWindow *window)
	{
		// Check mouse is inside rectangle
		bool mouseInside = m_Rect.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(*window)));

		// Nice highlighting for on hover
		if (mouseInside || m_Active)
			m_Rect.setFillColor(sf::Color(0x6E2F9DFF));
		else
			m_Rect.setFillColor(sf::Color(0x5E1F8DFF));

		// Change state depending on what the button should do
		if (Program::s_Instance->WasMousePressed())
		{
			if (mouseInside)
			{
				m_Active = true;
				LOG_TRACE("Text Input Field is active");

				m_Rect.setFillColor(sf::Color(0x6E2F9DFF));
			}
			else if (m_Active)
			{
				m_Active = false;

				LOG_TRACE("Text Input Field is deactivated")	
				m_Rect.setFillColor(sf::Color(0x5E1F8DFF));
			}
		}
	}

	void TextInput::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		target.draw(m_Rect, states);
		target.draw(m_Text, states);
	}

	void TextInput::CenterTextLeft()
	{
		// Center Text in rectangle due to position being top left corner of object
		sf::Vector2f textPosition = m_Rect.getPosition();
		textPosition.x += 5.0f;
		textPosition.y += 0.5f * (m_Rect.getSize().y - m_Text.getGlobalBounds().getSize().y) - (m_Text.getCharacterSize() >> 2);
		m_Text.setPosition(textPosition);
	}
}