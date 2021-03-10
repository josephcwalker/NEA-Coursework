#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Tools/Logger.h"

#include "Program/Program.h"
#include "Program/States/MainMenuState.h"

int main()
{
	// Create logger object
	Connect::Logger logger("Connect 4");
	logger.SetLogLevel(Connect::LOG_LEVEL::TRACE);

	// Create new program and use main menu as the inital state
	Connect::Program program;
	program.PushState(new Connect::MainMenuState());

	while (program.isRunning())
		program.ExecuteFrame();
		
	return 0;
}