#include "Program.h"

#include "Tools/Logger.h"
#include "States/ConfirmExitState.h"

namespace Connect
{
	Program* Program::s_Instance = nullptr;
	sf::Font* Program::s_Font = nullptr;

	Program::Program()
	{
		if (s_Instance)
		{
			LOG_WARN("Program object created multiple times");
			return;
		}
		else
			s_Instance = this;

		// Create Window and give pointer to it for all states
		LOG_TRACE("New Window created");

		m_Window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Connect 4 With AI", sf::Style::Titlebar | sf::Style::Close);

		if (!m_Window)
		{
			LOG_ERROR("SFML Window could not be created");
		}
		else
		{
			LOG_TRACE("Window framerate changed to 60");
			m_Window->setFramerateLimit(60);

			// Load global font
			s_Font = new sf::Font();
			LOG_TRACE("Loading Font From File");
			if (!s_Font->loadFromFile("res/font/Calibri-Regular.ttf"))
				LOG_ERROR("Error whilst loading font from file");
		}
	}

	Program::~Program()
	{
		while (!m_StateStack.empty())
			PopState();
		
		m_Window->close();
		
		LOG_TRACE("Removed all states from stack");
		LOG_TRACE("Called Program destructor");
	}

	void Program::PushState(State* newState)
	{
		newState->SetWindow(m_Window);
		LOG_TRACE("Added new state to top of state stack");
		m_StateStack.push(newState);
		LOG_TRACE("Initialized new state");
		m_StateStack.top()->Initialize();
	}

	void Program::PopState()
	{
		m_StatesToPop++;
	}

	void Program::RemoveAllStates()
	{
		m_StatesToPop = -1;
	}

	void Program::ExecuteFrame()
	{
		m_StateStack.top()->Execute();

		// Pop required states
		if (m_StatesToPop == -1)
		{
			LOG_TRACE("Removing all States");
			m_StatesToPop = m_StateStack.size();
		}
		
		for (int i = 0; i < m_StatesToPop; i++)
		{
			LOG_TRACE("Deleting current state and removing from stack");
			delete m_StateStack.top();
			m_StateStack.pop();
		}

		m_StatesToPop = 0;

		m_WasMousePressed = false;

		// Handle Input
		sf::Event e;
		while (m_Window->pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				LOG_TRACE("Window Close callback called");
				if (dynamic_cast<ConfirmExitState*>(m_StateStack.top()) == NULL)
					PushState(new ConfirmExitState());
				break;
			case sf::Event::MouseButtonPressed:
				LOG_TRACE("Mouse Button Pressed Event Occurred");
				m_WasMousePressed = true;
				break;
			}
		}

		// Clear, Draw, Display cycle
		m_Window->clear(sf::Color(0x121212FF));
		if (!m_StateStack.empty())
			m_StateStack.top()->Draw();
		m_Window->display();
	}
}