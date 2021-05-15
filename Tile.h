#pragma once

#include <SFML\Graphics.hpp>

class Tile
{
public:

	int x, y, type;
	bool colider = false;

	bool occupied = false;
	bool will_be_occupied = false;

	sf::Color color;

	Tile();
	~Tile();
};

