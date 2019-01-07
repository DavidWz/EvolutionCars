#include "World.h"

#include <random>
#include <chrono>

const float World::baseHeight = 100.0f;
const float World::gravity = -9.8f;

/*
Initializes one Segment of the terrain.
terrain: the list of terrain segments
terrainBody: the body of the terrain
left: the left x coordinate
right: the right x coordinate
leftH: the left height
right: the right height
*/
void initSegment(std::vector<sf::ConvexShape>& terrain, b2Body* terrainBody, float left, float right, float leftH, float rightH) {
	// create the convex shape of the terrain segment
	sf::ConvexShape shape(4);
	shape.setPoint(0, sf::Vector2f(left, 0));
	shape.setPoint(1, sf::Vector2f(left, leftH));
	shape.setPoint(2, sf::Vector2f(right, rightH));
	shape.setPoint(3, sf::Vector2f(right, 0));
	terrain.push_back(shape);

	b2Vec2 verts[4];
	verts[0].Set(left, 0);
	verts[1].Set(left, leftH);
	verts[2].Set(right, rightH);
	verts[3].Set(right, 0);
	b2PolygonShape terrainShape;
	terrainShape.Set(verts, 4);

	// create a new fixture and add it to the body
	b2FixtureDef terrainFixtureDef;
	terrainFixtureDef.shape = &terrainShape;
	terrainFixtureDef.density = 1;
	terrainBody->CreateFixture(&terrainFixtureDef);
}

World::World(float width, float padding, float maxHeight, int numSegments, float variance)
	: world(b2Vec2(0.0f, gravity)),
	width(width)
{
	height = baseHeight;
	float segWidth = (width - 2*padding) / (float) numSegments;

	b2BodyDef terrainBodyDef;
	terrainBodyDef.type = b2_staticBody;
	terrainBodyDef.position.Set(0, 0);
	terrainBodyDef.angle = 0;

	terrainBody = world.CreateBody(&terrainBodyDef);

	// start area
	initSegment(terrain, terrainBody, 0, padding, baseHeight, baseHeight);

	// terrain area
	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::normal_distribution<float> heightDist(0.0, variance);
	
	// create every segment
	float lastHeight = baseHeight;
	for (int i = 0; i < numSegments; i++) {
		// generate the next height which differs according to a normal distribution with given variance around the current height
		float nextHeight;
		do {
			float diff = heightDist(generator);
			nextHeight = lastHeight + diff;
		} while (nextHeight < baseHeight || nextHeight > maxHeight);

		if (nextHeight > height) {
			height = nextHeight;
		}

		initSegment(terrain, terrainBody, padding + i * segWidth, padding + (i+1) * segWidth, lastHeight, nextHeight);

		lastHeight = nextHeight;
	}

	// finish area
	initSegment(terrain, terrainBody, width-padding, width, lastHeight, lastHeight);

	// style
	for (auto& segment : terrain) {
		segment.setFillColor(sf::Color(210, 105, 30));
	}
}

void World::update(float second) {
	world.Step(second, velocityIterations, positionIterations);
}

void World::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (auto& segment : terrain) {
		target.draw(segment, states);
	}
}

b2World& World::getWorld() {
	return world;
}

float World::getWidth() const {
	return width;
}

float World::getHeight() const {
	return height;
}