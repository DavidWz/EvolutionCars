#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

#include "World.h"
#include "Car.h"
#include "Population.h"
#include "Stats.h"
#include "Camera.h"

bool displayGraphics = true;

/*
Handles the event loop of the window (closing, resizing, user input)
 */
bool handleEventLoop(sf::RenderWindow& window) {
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			// end the program
			return false;
		}
		else if (event.type == sf::Event::Resized)
		{
			// adjust the viewport when the window is resized
			glViewport(0, 0, event.size.width, event.size.height);
		}
		else if (event.type == sf::Event::KeyPressed) {
			// if the user presses space, we switch between showing the world and displaying statistics
			if (event.key.code == sf::Keyboard::Space) {
				displayGraphics = !displayGraphics;
			}
		}
	}
	return true;
}

int main()
{
	// graphics
	float padding = 100.0f;
	float width = 3000.0f;
	sf::RenderWindow window(sf::VideoMode(1200, 720), "Car Evolution");
	Camera cam(window.getSize());
	cam.setPosition(sf::Vector2f(padding / 2.0f, World::baseHeight));

	// create a new world, with a new population
	World world(width, padding, 500.0f, 500, 5.0f);
	Population population(b2Vec2(10, 100), width - padding / 2.0f, world.getWorld(), 30);
	Stats stats(world, population);

	// game loop
	float TARGET_FPS = 60.0f;

	sf::Clock clock;
	sf::Time timeStep = sf::seconds(1.0f / TARGET_FPS);

	sf::Time currentTime = clock.getElapsedTime();
	sf::Time accumulator = sf::seconds(0);

	bool running = true;
	while (running)
	{
		// event loop
		running = handleEventLoop(window);

		sf::Time newTime = clock.getElapsedTime();
		sf::Time frameTime = newTime - currentTime;
		currentTime = newTime;

		/*
		There are two display modes:
		1. Show the terrain with the cars.
		2. Show the statistics.
		While statistics are shown, the simulation is progressed much faster than real-time.
		*/
		if (!displayGraphics) {
			// simulate the world faster than normal (in this case FPS times faster)
			for (int i = 0; i < TARGET_FPS; i++) {
				world.update(timeStep.asSeconds());
				population.update(timeStep.asSeconds());
			}

			// draw the stats
			window.clear(sf::Color(200, 220, 255));
			window.draw(stats);
			window.display();
		}
		else {
			accumulator += frameTime;

			// physics
			while (accumulator >= timeStep)
			{
				world.update(timeStep.asSeconds());
				population.update(timeStep.asSeconds());
				accumulator -= timeStep;
			}

			// camera
			cam.update(frameTime.asSeconds());
			cam.moveTo(population.getFurthestActiveCar().getPosition());

			// graphics
			window.clear(sf::Color(200, 220, 255));

			window.draw(population, cam.getTransform());
			window.draw(world, cam.getTransform());

			window.display();
		}

		// game logic
		if (population.isDead()) {
			stats.log();
			population.createNext();
		}
	}

	return 0;
}