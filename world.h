#pragma once

#include "Tile.h"
#include <random>
#include "Entity.h"
#include <vector>

class world
{
public:

	static const int size = 40;

	Tile tiles[size][size];

	std::vector<Entity> entities;
	std::vector<Entity> tempentities;

	int entity_emount = 0;

	//change amount to spawn in constructor
	int amount_to_spawn = 10;

	int food_number = 0;

	float regrow_time = 10;

	world();
	~world();
	void create();
	void updateTiles(sf::Clock& r);
	void drawTiles(sf::RenderWindow& win);
	void howManyEntities();
	void spawnEntities();
	void moveEntities(sf::Clock& c, sf::Clock& a);
	void drawEntities(sf::RenderWindow& win);
	void checkForDeadEntity();
};

