#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace Connect
{
	class Title : public sf::Drawable
	{
	public:
		Title(sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), std::string text = "Default Text");
		virtual ~Title();

	public:
		void SetPosition(sf::Vector2f position);
		void SetCharacterSize(unsigned int size);
		void SetText(std::string text);

	private:
		// Override draw method to draw both rect and text
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		sf::Text m_Text;

		sf::Font m_Font;
	};
}