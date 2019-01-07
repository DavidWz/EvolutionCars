#include "Population.h"

#include <iostream>
#include <random>
#include <chrono>

Population::Population(b2Vec2 startPos, float endDistance, b2World& world, int size) :
	startPos(startPos),
	endDistance(endDistance),
	world(world),
	size(size),
	generation(1)
{
	for (int i = 0; i < size; i++) {
		cars.push_back(Car(CarDNA(), startPos, endDistance, world));
	}
}

void Population::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (const Car& car : cars) {
		car.draw(target, states);
	}
}

void Population::update(float elapsed) {
	for (Car& car : cars) {
		car.update(elapsed);
	}
}

Car& Population::getFurthestActiveCar() {
	int bestCar = 0;

	for (int i = 0; i < cars.size(); i++) {
		if (cars[i].isDead()) {
			continue;
		}
		if (cars[i].getPosition().x > cars[bestCar].getPosition().x || cars[bestCar].isDead()) {
			bestCar = i;
		}
	}

	return cars[bestCar];
}

bool Population::isDead() const {
	for (const Car& car : cars) {
		if (!car.isDead()) {
			return false;
		}
	}
	return true;
}

void Population::printStats() const {
	for (const Car& car : cars) {
		std::cout << generation << ", " << car.getFitness() << std::endl;
	}
}

void Population::createNext() {
	// find best car
	int bestCar = 0;
	for (int i = 1; i < size; i++) {
		if (cars[i].getFitness() > cars[bestCar].getFitness()) {
			bestCar = i;
		}
	}

	// create mating pool
	std::vector<CarDNA> matingPool;
	for (const Car& car : cars) {
		int n = (int) floor(car.getFitness() / cars[bestCar].getFitness() * 100.0f);
		for (int i = 0; i < n; i++) {
			matingPool.push_back(car.getDNA());
		}
	}

	CarDNA bestDNA = cars[bestCar].getDNA();

	// destroy old cars
	for (int i = 0; i < size; i++) {
		cars[i].destroy();
	}
	cars.clear();

	// keep the best car
	cars.push_back(Car(bestDNA, startPos, endDistance, world));

	// select and mate parents
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> dist(0, matingPool.size()-1);

	for (int i = 1; i < size; i++) {
		CarDNA parentA = matingPool[dist(generator)];
		CarDNA parentB = matingPool[dist(generator)];
		CarDNA childDna = parentA.cross(parentB);
		cars.push_back(Car(childDna, startPos, endDistance, world));
	}

	generation++;
}

std::vector<Car>& Population::getCars() {
	return cars;
}