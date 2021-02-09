#pragma once

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Program/State.h"

namespace Connect
{
	class TextInput : public sf::Drawable
	{
	public:
		TextInput(State *parentState, bool passwordField = false, std::string text = "Default Text");
		virtual ~TextInput();

	public:
		void SetPosition(sf::Vector2f position);
		void SetSize(sf::Vector2f size);
		void SetText(std::string text);

		void KeyboardEvent();
		void OnMouseUpdate(sf::RenderWindow *window);

		inline const std::string& GetInput() { return m_UserInput; }

	private:
		// Override draw method to draw both rect and text
		void draw(sf::RenderTarget &target, sf::RenderStates states) const;

		// Recenter Text
		void CenterTextLeft();

	private:
		sf::RectangleShape m_Rect;
		sf::Text m_Text;

		std::string m_UserInput;
		std::string m_DefaultText;

		bool m_Active = false;

		bool m_PasswordInputField = false;

	public:
		State* m_ParentState = nullptr;
	};
}