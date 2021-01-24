#include "Program.h"

#include "Tools/Logger.h"

namespace Connect
{
	Program* Program::s_Instance = nullptr;

	Program::Program(State* initialState)
	{
		if (s_Instance)
		{
			LOG_WARN("Program object created multiple times");
			return;
		}
		else
			s_Instance = this;

		LOG_TRACE("New Program object created");

		m_StateStack.push(initialState);

		// Create Window and give pointer to it for all states
		LOG_TRACE("New Window created");

		m_Window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Connect 4 With AI", sf::Style::Default);

		if (!m_Window)
		{
			LOG_ERROR("SFML Window could not be created");
		}
		else
		{
			LOG_TRACE("Window framerate changed to 60");
			m_Window->setFramerateLimit(60);
			initialState->SetWindow(m_Window);
		}
	}

	Program::~Program()
	{
		while (!m_StateStack.empty())
			PopState();
		
		LOG_TRACE("Removed all states from stack");
		LOG_TRACE("Called Program destructor");
	}

	void Program::PushState(State* newState)
	{
		LOG_TRACE("Added new state to top of state stack");
		m_StateStack.push(newState);
		LOG_TRACE("Initialized new state");
		m_StateStack.top()->Initialize();
	}

	void Program::PopState()
	{
		LOG_TRACE("Deleting current state and removing from stack");
		delete m_StateStack.top();
		m_StateStack.pop();
	}

	void Program::ExecuteFrame()
	{
		m_StateStack.top()->Execute();

		m_Window->clear(sf::Color(0x00000000));
		m_StateStack.top()->Draw();
		m_Window->display();
	}
}