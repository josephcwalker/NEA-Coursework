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
		m_StateStack.top()->Draw();
	}
}