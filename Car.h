#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "CarDNA.h"

/*
The graphics object for one car.
*/
class CarGraphics
	: public sf::Drawable
{
private:
	sf::ConvexShape carShape;
	sf::CircleShape rearWheelShape;
	sf::CircleShape frontWheelShape;
	sf::RectangleShape rearWheelLine;
	sf::RectangleShape frontWheelLine;
public:
	CarGraphics(CarDNA dna);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	void update(float rearRot, float frontRot, bool isDead);
private:
	void initGraphics(CarDNA dna);
};

/*
A car including graphics and physics.
*/
class Car
	: public sf::Drawable
{
private:
	static const float CAR_DENSITY;
	static const float CAR_FRICTION;
	static const float CAR_RESTITUTION;
	static const float WHEEL_DENSITY;
	static const float WHEEL_FRICTION;
	static const float WHEEL_RESTITUTION;
	static const float WHEEL_MOTOR_MAXTORQUE;

	CarDNA dna;

	b2World& world;
	b2Body* carBody;
	b2Body* rearWheelBody;
	b2RevoluteJoint* rearWheelJoint;
	b2Body* frontWheelBody;
	b2RevoluteJoint* frontWheelJoint;

	CarGraphics graphics;

	float bestDistance;
	float timeToBestDistance;
	float timeSinceLastImprovement;
	float timeAlive;
	static const float TIME_TO_DEATH;

	float endDistance;
public:
	Car(CarDNA dna, b2Vec2 startPos, float endDistance, b2World& world);
	void update(float elapsed);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	sf::Vector2f getPosition() const;
	bool isDead() const;
	float getFitness() const;
	CarDNA getDNA() const;
	void destroy();
	float getBestDistance() const;
	bool operator <(const Car& other) const;
	Car& operator =(const Car &);
private:
	void initBody(b2Vec2 pos);
	void initWheels(b2Vec2 pos);
};

