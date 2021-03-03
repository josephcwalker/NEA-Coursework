#pragma once

#include "State.h"

#include <stack>
#include <queue>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace Connect
{
	class Program
	{
	public:
		Program();
		virtual ~Program();

	public:
		// Add or remove states from the stack
		void PushState(State* newState);
		void PopState();

		void RemoveAllStates();

		// Useful methods
		inline State* Top() { return m_StateStack.top(); }
		inline bool isRunning() { return !m_StateStack.empty() || !m_StatesToPush.empty(); }

		// Will be run every frame to compute and draw
		// Program will be locked at 60fps so 1/60 is the time between frames
		void ExecuteFrame();

		inline bool WasMousePressed() { return m_WasMousePressed; }

	private:

		void ExecuteStateChanges();

	public:
		static Program* s_Instance;

		// Global Font for all objects to use
		static sf::Font* s_Font;

		static sf::Keyboard::Key s_KeyPressed;

	private:
		// Holds all currently active states
		std::stack<State*> m_StateStack;
		// Window to draw objects to
		sf::RenderWindow* m_Window;

		bool m_WasMousePressed = false;

		// Number of times PopState was called
		// Used after to keep variables in memory
		int m_StatesToPop = 0;

		// States pushed during the current frame
		std::queue<State*> m_StatesToPush;
	};
}