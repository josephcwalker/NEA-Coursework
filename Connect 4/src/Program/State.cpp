#include "State.h"

#include "Program.h"
#include "States/ConfirmExitState.h"

sf::RenderWindow* Connect::State::m_Window = nullptr;

void Connect::State::PopState()
{
	Program::s_Instance->PopState();
}

void Connect::State::RemoveAllStates()
{
	Program::s_Instance->RemoveAllStates();
}

void Connect::State::PushConfirmExitState()
{
	Program::s_Instance->PushState(new Connect::ConfirmExitState());
}