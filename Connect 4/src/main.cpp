#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Tools/Logger.h"

int main()
{
	Connect::Logger logger("Window");
	logger.SetLogLevel(Connect::LOG_LEVEL::TRACE);

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Hello");
	if (!window)
	{
		LOG_ERROR("Error when creating new SFML render window");
		return -1;
	}

	LOG_TRACE("Window framerate changed to 60");
	window->setFramerateLimit(60);

	while (window->isOpen())
	{
		window->display();
	}

	return 0;
}