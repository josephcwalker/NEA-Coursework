#pragma once

#include "State.h"

#include <stack>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

namespace Connect
{
	class Program
	{
	public:
		Program(State* initialState);
		virtual ~Program();

	public:
		// Add or remove states from the stack
		void PushState(State* newState);
		void PopState();

		// Useful methods
		inline State* Top() { return m_StateStack.top(); }
		inline bool isRunning() { return !m_StateStack.empty(); }

		// Will be run every frame to compute and draw
		// Program will be locked at 60fps so 1/60 is the time between frames
		void ExecuteFrame();

	public:
		static Program* s_Instance;

	private:
		// Holds all currently active states
		std::stack<State*> m_StateStack;
		// Window to draw objects to
		sf::RenderWindow* m_Window;
	};
}