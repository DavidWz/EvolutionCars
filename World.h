#pragma once

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

/*
World class which contains the terrain and the physical world object.
*/
class World
	: public sf::Drawable
{
private:
	const int velocityIterations = 8;
	const int positionIterations = 3;
	b2World world;

	std::vector<sf::ConvexShape> terrain;
	b2Body* terrainBody;
	float height;
	float width;
public:
	static const float baseHeight;
	static const float gravity;

	/*
	Creates a new world with a certain width, left- and right padding, maximum height, 
	number of segments (spikes) and a variance in height between every spike.
	*/
	World(float width, float padding, float maxHeight, int numSegments, float variance);
	void update(float second);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	b2World& getWorld();
	float getWidth() const;
	float getHeight() const;
};

