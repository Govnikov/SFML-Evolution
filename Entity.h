#pragma once

#include <SFML\Graphics.hpp>
#include <vector>
#include "Tile.h"
#include <iostream>

class Entity
{
public:
	int x, y, prevx, prevy, nextx, nexty;

	int xsize = 15;
	int ysize = 15;

	static const int world_tile_size = 40;

	sf::Color color;

	int num_of_same_colors;

	int damage;
	int health;
	int maxhealth;

	int hunger;
	int maxhunger;
	int goingx;
	int goingy;

	int how_long_survived = 0;
	
	int time_to_mutate = 50;

	float attack_time = 1;

	bool can_go_left = true;
	bool can_go_right = true;
	bool can_go_up = true;
	bool can_go_down = true;

	bool is_patrolling = true;
	bool is_following = false;
	bool going_for_food = false;

	bool time_to_birth = false;

	bool running_from_target = false;

	int see_radius = 3;

	Entity* selfptr = this;
	Entity* parent = nullptr;
	Entity* followingptr = nullptr;

	std::vector<Entity*> children;

	Entity();
	~Entity();

	void randomColor();

	void draw(sf::RenderWindow& win);
	void checkIfCanGo(Tile t[world_tile_size][world_tile_size]);

	void checkRadius(Entity* other);
	void patrolling(sf::Clock& c, Tile t[world_tile_size][world_tile_size]);
	void following(sf::Clock& c, Tile t[world_tile_size][world_tile_size]);
	void runningAway(sf::Clock& c, Tile t[world_tile_size][world_tile_size]);
	void move(sf::Clock& c, Entity *other, Tile t[world_tile_size][world_tile_size]);

	void attack(Entity *other, sf::Clock& a);

	void checkFoodRadius(Tile t[world_tile_size][world_tile_size]);
	void goingForFood(sf::Clock& c, Tile t[world_tile_size][world_tile_size]);
	void starving(sf::Clock& c);
	void eat(Tile t[world_tile_size][world_tile_size]);

	void mutate();
};

