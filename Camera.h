#pragma once

#include <SFML/Graphics.hpp>

/*
Camera class which looks at the terrain and cars.
*/
class Camera
{
private:
	static const float secondsForMove;
	float speed;
	static const float epsilon;

	float width;
	float height;
	sf::Vector2f targetPosition;
	sf::Vector2f currentPosition;
public:
	Camera(sf::Vector2u windowSize);

	// return the camera transform matrix
	sf::Transform getTransform() const;

	// sets the center position of the camera
	void setPosition(sf::Vector2f pos);

	// moves the camera to a new position
	void moveTo(sf::Vector2f position);

	// updates the camera in real-time to enable smooth movement
	void update(float second);
};

