#include "Entity.h"



Entity::Entity()
{
	if (parent == nullptr)
	{
		randomColor();
	}
	this->x = 0;
	this->y = 0;
	this->prevx = 0;
	this->prevy = 0;
}


Entity::~Entity()
{
}

void Entity::randomColor()
{
	int rand1 = rand() % 255 + 1;
	int rand2 = rand() % 255 + 1;
	int rand3 = rand() % 255 + 1;

	color = sf::Color(rand1, rand2, rand3);
}

void Entity::draw(sf::RenderWindow & win)
{
	sf::RectangleShape shape;
	shape.setFillColor(color);
	shape.setPosition(x * xsize, y * ysize);
	shape.setSize(sf::Vector2f(xsize, ysize));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color::Black);
	win.draw(shape);
}

void Entity::checkIfCanGo(Tile t[world_tile_size][world_tile_size])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (t[i][j].x == x - 1 && t[i][j].y == y)
			{
				if (t[i][j].will_be_occupied || t[i][j].occupied)
				{
					can_go_left = false;
				}
				else
				{
					can_go_left = true;
				}
			}
			if (t[i][j].x == x + 1 && t[i][j].y == y)
			{
				if (t[i][j].will_be_occupied || t[i][j].occupied)
				{
					can_go_right = false;
				}
				else
				{
					can_go_right = true;
				}
			}
			if (t[i][j].x == x && t[i][j].y == y - 1)
			{
				if (t[i][j].will_be_occupied || t[i][j].occupied)
				{
					can_go_up = false;
				}
				else
				{
					can_go_up = true;
				}
			}
			if (t[i][j].x == x && t[i][j].y == y + 1)
			{
				if (t[i][j].will_be_occupied || t[i][j].occupied)
				{
					can_go_down = false;
				}
				else
				{
					can_go_down = true;
				}
			}
			
		}
	}
}

void Entity::checkRadius(Entity* other)
{
	if (other != this && health >= maxhealth / 2 && other != nullptr && color != other->color)
	{
		if ((x + see_radius) >= other->x && (x - see_radius) <= other->x || (y + see_radius) >= other->y && (y - see_radius) <= other->y)
		{
			is_patrolling = false;
			is_following = true;
			followingptr = other;
			if (other->damage >= damage * 2)
			{
				running_from_target = true;
			}
		}
		else
		{
			followingptr = nullptr;
		}
	}
	if (other != nullptr)
	{
		num_of_same_colors = 0;
		if (other->color == color)
		{
			num_of_same_colors++;
		}
	}
}

void Entity::patrolling(sf::Clock & c, Tile t[world_tile_size][world_tile_size])
{
	float change_dir = rand() % 2 + 1;
	if (c.getElapsedTime().asSeconds() >= change_dir)
	{
		int rand_dir = rand() % 4;
		if (rand_dir == 0 && can_go_left && x > 0 && x < world_tile_size-1)//left
		{
			prevx = x;
			t[x - 1][y].will_be_occupied = true;
			x -= 1;
			t[x][y].occupied = true;
			t[x][y].will_be_occupied = false;
		}
		if (rand_dir == 0 && !can_go_left && x > 0 && x < world_tile_size-1)
		{
			rand_dir = rand() % 4;
		}

		if (rand_dir == 1 && can_go_right && x < world_tile_size-1 && x > 0)//right
		{
			prevx = x;
			t[x + 1][y].will_be_occupied = true;
			x += 1;
			t[x][y].occupied = true;
			t[x][y].will_be_occupied = false;
		}
		if (rand_dir == 1 && !can_go_right && x > 0 && x < world_tile_size-1)
		{
			rand_dir = rand() % 4;
		}

		if (rand_dir == 2 && can_go_up && y > 0 && y < world_tile_size-1)//up
		{
			prevy = y;
			t[x][y - 1].will_be_occupied = true;
			y -= 1;
			t[x][y].occupied = true;
			t[x][y].will_be_occupied = false;
		}
		if (rand_dir == 2 && !can_go_up && y > 0 && y < world_tile_size-1)
		{
			rand_dir = rand() % 4;
		}

		if (rand_dir == 3 && can_go_down && y < world_tile_size-1 && y > 0)//down
		{
			prevy = y;
			t[x][y + 1].will_be_occupied = true;
			y += 1;
			t[x][y].occupied = true;
			t[x][y].will_be_occupied = false;
		}
		if (rand_dir == 3 && !can_go_down && y > 0 && y < world_tile_size-1)
		{
			rand_dir = rand() % 4;
		}

	}
}

void Entity::following(sf::Clock & c, Tile t[world_tile_size][world_tile_size])
{
	if (followingptr != nullptr)
	{
		if (c.getElapsedTime().asSeconds() >= 1)
		{
			if (x < followingptr->x && can_go_right && x < world_tile_size-1)
			{
				prevx = x;
				t[x + 1][y].will_be_occupied = true;
				x++;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
			else if (x > followingptr->x && can_go_left && x > 0)
			{
				prevx = x;
				t[x - 1][y].will_be_occupied = true;
				x--;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
			else if (y < followingptr->y && can_go_down && y < world_tile_size-1)
			{
				prevy = y;
				t[x][y+1].will_be_occupied = true;
				y++;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
			else if (y > followingptr->y && can_go_up && y > 0)
			{
				prevy = y;
				t[x][y - 1].will_be_occupied = true;
				y--;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
		}
	}
	else
	{
		is_following = false;
		is_patrolling = true;
		going_for_food = false;
	}
}

void Entity::runningAway(sf::Clock & c, Tile t[world_tile_size][world_tile_size])
{
	if (followingptr != nullptr)
	{
		if (c.getElapsedTime().asSeconds() >= 1)
		{
			if (x < followingptr->x && can_go_right && x > 0)
			{
				prevx = x;
				t[x + 1][y].will_be_occupied = true;
				x--;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
			else if (x > followingptr->x && can_go_left && x > world_tile_size - 1)
			{
				prevx = x;
				t[x - 1][y].will_be_occupied = true;
				x++;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
			else if (y < followingptr->y && can_go_down && y < 0)
			{
				prevy = y;
				t[x][y + 1].will_be_occupied = true;
				y--;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
			else if (y > followingptr->y && can_go_up && y > world_tile_size - 1)
			{
				prevy = y;
				t[x][y - 1].will_be_occupied = true;
				y++;
				t[x][y].occupied = true;
				t[x][y].will_be_occupied = false;
			}
		}
	}
	else
	{
		is_following = false;
		is_patrolling = true;
		going_for_food = false;
		running_from_target = false;
	}
}


void Entity::move(sf::Clock& c, Entity* other, Tile t[world_tile_size][world_tile_size])
{
	eat(t);
	if (is_patrolling == true)
	{
		patrolling(c, t);
		checkRadius(other);
		checkFoodRadius(t);
	}
	if (is_following == true)
	{
		if (running_from_target == false)
		{
			following(c, t);
		}
		if (running_from_target == true)
		{
			runningAway(c, t);
		}
		checkRadius(other);
		checkFoodRadius(t);
	}
	if (going_for_food)
	{
		checkFoodRadius(t);
		goingForFood(c, t);
	}
	starving(c);
}

void Entity::attack(Entity* other, sf::Clock& a)
{
	if (followingptr == other && followingptr != nullptr)
	{
		if (a.getElapsedTime().asSeconds() >= attack_time)
		{
			if (x - 1 == other->x && y == other->y ||
				x + 1 == other->x && y == other->y ||
				x == other->x && y - 1 == other->y ||
				x == other->x && y + 1 == other->y ||
				x - 1 == other->x && y - 1 == other->y ||
				x - 1 == other->x && y + 1 == other->y ||
				x + 1 == other->x && y - 1 == other->y ||
				x + 1 == other->x && y + 1 == other->y ||
				x == other->x && y == other->y)
			{
				other->health = other->health - damage;
			}
		}
	}
}

void Entity::checkFoodRadius(Tile t[world_tile_size][world_tile_size])
{
	if (health <= maxhealth / 2 || hunger <= maxhunger / 2)
	{
		for (int i = 0; i < world_tile_size; i++)
		{
			for (int j = 0; j < world_tile_size; j++)
			{
				if (x + see_radius == t[i][j].x && y + see_radius == t[i][j].y && t[i][j].type == 1)
				{
					followingptr = nullptr;
					is_following = false;
					is_patrolling = false;
					going_for_food = true;
					goingx = t[i][j].x;
					goingy = t[i][j].y;
				}
			}
		}
	}
	else
	{
		going_for_food = false;
		is_patrolling = true;
		is_following = false;
	}
}

void Entity::goingForFood(sf::Clock& c, Tile t[world_tile_size][world_tile_size])
{
	if (health <= maxhealth / 2 || hunger <= maxhunger / 2 && going_for_food)
	{
		if (goingx >= 0 && goingx <= 49)
		{
			if (goingy >= 0 && goingy <= 49)
			{
				if (c.getElapsedTime().asSeconds() >= 1)
				{
					if (x < goingx && can_go_right && x < world_tile_size-1)
					{
						prevx = x;
						t[x + 1][y].will_be_occupied = true;
						x++;
						t[x][y].occupied = true;
						t[x][y].will_be_occupied = false;
					}
					else if (x > goingx && can_go_left && x > 0)
					{
						prevx = x;
						t[x - 1][y].will_be_occupied = true;
						x--;
						t[x][y].occupied = true;
						t[x][y].will_be_occupied = false;
					}
					else if (y < goingy && can_go_down && y < world_tile_size-1)
					{
						prevy = y;
						t[x][y + 1].will_be_occupied = true;
						y++;
						t[x][y].occupied = true;
						t[x][y].will_be_occupied = false;
					}
					else if (y > goingy && can_go_up && y > 0)
					{
						prevy = y;
						t[x][y - 1].will_be_occupied = true;
						y--;
						t[x][y].occupied = true;
						t[x][y].will_be_occupied = false;
					}
				}
			}
		}
	}
}

void Entity::starving(sf::Clock & c)
{
	if (c.getElapsedTime().asSeconds() >= 1)
	{
		int chance = rand() % 3;
		if (chance == 1)
		{
			hunger--;
		}
	}
	if (hunger <= 0)
	{
		if (c.getElapsedTime().asSeconds() >= 1)
		{
			if (health > 0)
			{
				int chance = rand() % 3;
				if (chance == 1)
				{
					hunger--;
				}
			}
		}
	}
}

void Entity::eat(Tile t[world_tile_size][world_tile_size])
{
	if (x >= 0 && x <= 49 && y >= 0 && y <= 49)
	{
		if (goingx == x && goingy == y)
		{
			if (t[x][y].type == 1)
			{
				t[x][y].type = 0;
				health = maxhealth;
				going_for_food = false;

				int birth_chane = rand() % 1+1;
				if (birth_chane == 1)
				{
					time_to_birth = true;
				}
			}
		}
	}
}

void Entity::mutate()
{
	if (how_long_survived%time_to_mutate == 0)
	{
		int num_of_mutations = rand() % 6 + 1;
		for (int i = 0; i < num_of_mutations; i++)
		{
			int to_mutate = rand() % 6;
			//mutate damage
			if (to_mutate == 0)
			{
				int damage_to_add = rand() % 3 + 1;
				damage += damage_to_add;
			}
			//mutate health
			if (to_mutate == 1)
			{
				int health_to_add = rand() % 3 + 1;
				maxhealth += health_to_add;
			}
			//mutate hunger
			if (to_mutate == 2)
			{
				int hunger_to_add = rand() % 3 + 1;
				maxhunger += hunger_to_add;
			}
			//mutate mutation time
			if (to_mutate == 3 && time_to_mutate > 3)
			{
				int mutation_time_to_subtract = rand() % 3 + 1;
				time_to_mutate -= mutation_time_to_subtract;
			}
			//mutate attack time
			if (to_mutate == 4 && attack_time >0.1)
			{
				int attack_time_to_subtract = rand() % 5 + 1;
				float to_subtract = float(attack_time_to_subtract / 10);
				attack_time -= to_subtract;
			}
			//mutate see radius
			if (to_mutate == 5 && see_radius < 10)
			{
				int radius_to_increase = rand() % 2 + 1;
				see_radius += radius_to_increase;
			}
		}
		//full heal after mutation
		health = maxhealth;
	}
}

