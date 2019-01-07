#pragma once

#include <vector>
#include "Car.h"
#include "World.h"

/*
A population contains every car of one generation.
*/
class Population
	: public sf::Drawable
{
private:
	int size;

	std::vector<Car> cars;

	b2Vec2 startPos;
	float endDistance;
	b2World& world;

	int generation;
public:
	Population(b2Vec2 startPos, float endDistance, b2World& world, int size);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void update(float elapsed);
	Car& getFurthestActiveCar();
	bool isDead() const;
	void printStats() const;
	void createNext();
	std::vector<Car>& getCars();
};

