#pragma once

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Program/State.h"

namespace Connect
{
	typedef void (State::*onClickFunction)();

	class Button : public sf::Drawable
	{
	public:
		Button(onClickFunction function, State* parentState, std::string text = "Default Text");
		virtual ~Button();

	public:
		void SetPosition(sf::Vector2f position);
		void SetSize(sf::Vector2f size);
		void SetText(std::string text);

		void OnMouseUpdate(sf::RenderWindow* window);

	private:
		// Override draw method to draw both rect and text
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		// Recenter Text
		void CenterText();

	private:
		sf::RectangleShape m_Rect;
		sf::Text m_Text;

	public:
		onClickFunction m_ClickFunction = nullptr;
		State* m_ParentState = nullptr;
	};
}