#include "world.h"



world::world()
{
	create();
	amount_to_spawn = 60;
}


world::~world()
{
}

void world::create()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			tiles[i][j].x = i;
			tiles[i][j].y = j;
			tiles[i][j].type = rand() % 10;
		}
	}
}

void world::updateTiles(sf::Clock& r)
{
	if (r.getElapsedTime().asSeconds() >= regrow_time && food_number < size*size / 2 )
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (tiles[i][j].type != 1)
				{
					int chance = rand() % 15;
					if (chance == 4)
					{
						tiles[i][j].type = 1;
					}
				}
			}
		}
	}
	if (r.getElapsedTime().asSeconds() >= regrow_time)
	{
		r.restart();
	}
}

void world::drawTiles(sf::RenderWindow & win)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (tiles[i][j].type != 1)
			{
				tiles[i][j].color = sf::Color::Green;
			}
			else
			{
				food_number++;
				tiles[i][j].color = sf::Color::Magenta;
			}
			sf::RectangleShape shape;
			shape.setFillColor(tiles[i][j].color);
			shape.setSize(sf::Vector2f(15, 15));
			shape.setPosition(sf::Vector2f(i*15, j*15));
			win.draw(shape);
		}
	}
}

void world::howManyEntities()
{
	int how_many = 0;
	if (entities.empty())
	{
		how_many = 0;
	}
	else
	{
		for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			how_many++;
		}
	}
	entity_emount = how_many;
}

void world::spawnEntities()
{
	if (entity_emount < amount_to_spawn)
	{
		Entity e;
		e.x = rand() % size;
		e.y = rand() % size;
		e.health = rand() % 10 + 10;
		e.maxhealth = e.health;
		e.damage = rand() % 5 + 3;
		e.maxhunger = rand() % 5 + 5;
		e.hunger = e.maxhunger;
		entities.push_back(e);
	}
}

void world::moveEntities(sf::Clock& c, sf::Clock& a)
{
	if (!entities.empty())
	{
		int elem = 0;
		for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			if (entities.at(elem).time_to_birth == true)
			{
				Entity e;
				e.x = rand() % size;
				e.y = rand() % size;
				e.attack_time = entities.at(elem).attack_time;
				e.color = entities.at(elem).color;
				e.damage = entities.at(elem).damage;
				e.maxhealth = entities.at(elem).maxhealth;
				e.health = e.maxhealth;
				e.maxhunger = entities.at(elem).maxhunger;
				e.hunger = e.maxhunger;
				e.parent = &entities.at(elem);
				e.see_radius = entities.at(elem).see_radius;
				tempentities.push_back(e);

				entities.at(elem).time_to_birth = false;
			}
			entities.at(elem).mutate();
			entities.at(elem).checkIfCanGo(tiles);
			int elemelem = 0;
			/*for (std::vector<Entity>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				entities.at(elem).move(c, &entities.at(elemelem), tiles);
				entities.at(elem).attack(&entities.at(elemelem), a);
				elemelem++;
			}*/
			Entity* another = nullptr;
			for (std::vector<Entity>::iterator i = entities.begin(); i != entities.end(); ++i)
			{
				if (&entities.at(elemelem) != &entities.at(elem) && &entities.at(elem) != entities.at(elem).parent && entities.at(elem).health >= entities.at(elem).maxhealth / 2)
				{
					if ((entities.at(elem).x + entities.at(elem).see_radius) >= entities.at(elemelem).x && (entities.at(elem).x - entities.at(elem).see_radius) <= entities.at(elemelem).x || (entities.at(elem).y + entities.at(elem).see_radius) >= entities.at(elemelem).y && (entities.at(elem).y - entities.at(elem).see_radius) <= entities.at(elemelem).y)
					{
						another = &entities.at(elemelem);
					}
				}
				elemelem++;
			}
			entities.at(elem).move(c, another, tiles);
			entities.at(elem).attack(another, a);

			entities.at(elem).how_long_survived++;
			if (entities.at(elem).how_long_survived > 1000)
			{
				entities.at(elem).health = 0;
			}
			elem++;
		}
	}
	checkForDeadEntity();
	if (a.getElapsedTime().asSeconds() >= 3)
	{
		a.restart();
	}
	if (c.getElapsedTime().asSeconds() >= 1)
	{
		c.restart();
	}
	if (!tempentities.empty())
	{
		int elem = 0;
		for (std::vector<Entity>::iterator tempit = tempentities.begin(); tempit != tempentities.end(); ++tempit)
		{
			entities.push_back(tempentities.at(elem));
			elem++;
		}
		tempentities.clear();
	}
}

void world::drawEntities(sf::RenderWindow& win)
{
	if (!entities.empty())
	{
		int elem = 0;
		for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end(); ++it)
		{
			entities.at(elem).draw(win);
			elem++;
		}
	}
}

void world::checkForDeadEntity()
{
	for (std::vector<Entity>::iterator it = entities.begin(); it != entities.end();)
	{
		if (it->health <= 0)
		{
			it = entities.erase(it);
		}
		else
		{
			++it;
		}
	}
}
