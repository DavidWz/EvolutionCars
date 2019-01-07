#include "Camera.h"

#include <iostream>

const float Camera::secondsForMove = 0.3f;
const float Camera::epsilon = 0.1f;

Camera::Camera(sf::Vector2u windowSize) :
	width(windowSize.x),
	height(windowSize.y),
	currentPosition(sf::Vector2f(0, 0)),
	targetPosition(sf::Vector2f(0, 0))
{

}

sf::Transform Camera::getTransform() const {
	sf::Transform tf = sf::Transform::Identity;
	tf.scale(10.0f, 10.0f, width / 2.0f, height / 2.0f);
	tf.translate(0.0f, height);
	tf.scale(1.0f, -1.0f);
	tf.translate(-currentPosition);
	tf.translate(width / 2.0f, height / 2.0f);
	return tf;
}

void Camera::setPosition(sf::Vector2f pos) {
	currentPosition = pos;
	targetPosition = pos;
}

void Camera::moveTo(sf::Vector2f position) {
	targetPosition = position;

	// don't immediatly set the new position, but do it smoothly
	sf::Vector2f diff = targetPosition - currentPosition;
	float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
	speed = distance / secondsForMove;
}

void Camera::update(float elapsed) {
	// reduce the distance from the current position to the target position
	sf::Vector2f diff = targetPosition - currentPosition;
	float distance = sqrt(diff.x*diff.x + diff.y*diff.y);
	float newDistance = distance - elapsed * speed;

	if (newDistance < 0) {
		newDistance = 0;
	}

	if (newDistance > epsilon) {
		diff = diff / distance * newDistance;
		currentPosition = targetPosition - diff;
	}
	else {
		currentPosition = targetPosition;
	}
}