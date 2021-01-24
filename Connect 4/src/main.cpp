#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Tools/Logger.h"

#include "Program/Program.h"
#include "Program/States/MainMenuState.h"

int main()
{
	Connect::Logger logger("Connect 4");
	logger.SetLogLevel(Connect::LOG_LEVEL::TRACE);

	Connect::Program program(new Connect::MainMenuState());

	while (program.isRunning())
		program.ExecuteFrame();

	return 0;
}