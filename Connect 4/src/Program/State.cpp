#include "State.h"

#include "Program.h"

sf::RenderWindow* Connect::State::m_Window = nullptr;

void Connect::State::PopState()
{
	Program::s_Instance->PopState();
}