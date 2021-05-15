#include "Application.h"


Application::Application()
{
}


Application::~Application()
{
}

void Application::run()
{
	sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "window");

	view.setSize(screen_width, screen_height);
	view.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	
	world w;
	//add how agressive the entities are and behaviour depending on the level of agressivness
	//if there is lets say 3 of the same color and entity is only one of the color then he doesnt attack, runs away to survive
	//packs of groups so they dont attack each other
	while (window.isOpen())
	{
		//update dt
		dt = dtclock.restart().asSeconds();
		//update mouse pos
		mouse_pos_scr = sf::Mouse::getPosition();
		mouse_pos_win = sf::Mouse::getPosition(window);
		mouse_pos_view = window.mapPixelToCoords(mouse_pos_win);
		mouse_pos_grid.x = int(mouse_pos_view.x) / 30;
		mouse_pos_grid.y = int(mouse_pos_view.y) / 30;
		//events
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}

		}
		//update
		w.howManyEntities();
		w.spawnEntities();
		w.moveEntities(aitime, aiattack);
		w.updateTiles(regrow);

		//update keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		{
			view.move(-100.f * dt, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		{
			view.move(100.f*dt, 0.f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			view.move(0.f, -100.f*dt);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			view.move(0.f, 100.f*dt);
		}
		//
		window.clear();
		window.setView(view);
		//game
		w.drawTiles(window);
		w.drawEntities(window);
		//ui
		window.display();
	}
}
