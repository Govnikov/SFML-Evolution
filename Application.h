#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "world.h"

class Application
{
public:
	Application();
	~Application();

	static const int screen_width = 600;
	static const int screen_height = 600;

	sf::Vector2i mouse_pos_scr;
	sf::Vector2i mouse_pos_win;
	sf::Vector2f mouse_pos_view;
	sf::Vector2i mouse_pos_grid;

	float dt = 0;
	sf::Clock dtclock;

	sf::Clock aitime;
	sf::Clock aiattack;
	sf::Clock regrow;

	sf::View view;

	void run();
};

