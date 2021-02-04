#include "Program.h"

#include "Tools/Logger.h"
#include "States/ConfirmExitState.h"

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
			initialState->SetWindow(m_Window);

			LOG_TRACE("New Program object created");
			PushState(initialState);
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

	void Program::RemoveAllStates()
	{
		LOG_TRACE("Removing all States");
		while (isRunning())
			PopState();
	}

	void Program::ExecuteFrame()
	{
		m_StateStack.top()->Execute();

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
			default:
				break;
			}
		}

		m_Window->clear(sf::Color(0x121212FF));
		if (!m_StateStack.empty())
			m_StateStack.top()->Draw();
		m_Window->display();
	}
}