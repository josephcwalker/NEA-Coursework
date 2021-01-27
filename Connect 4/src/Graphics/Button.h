#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace Connect
{
	class Button : public sf::Drawable
	{
	public:
		Button(sf::Vector2f position = sf::Vector2f(0.0f, 0.0f), sf::Vector2f size = sf::Vector2f(100.0f, 100.0f), std::string text = "Default Text");
		virtual ~Button();

	public:
		void SetPosition(sf::Vector2f position);
		void SetSize(sf::Vector2f size);
		void SetText(std::string text);

	private:
		// Override draw method to draw both rect and text
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Recenter Text
		void CenterText();

	private:
		sf::RectangleShape m_Rect;
		sf::Text m_Text;

		sf::Font m_Font;
	};
}