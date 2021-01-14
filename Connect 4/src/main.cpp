#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Hello");
	window->setFramerateLimit(60);

	while (window->isOpen())
	{
		window->display();
	}

	return 0;
}