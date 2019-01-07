#include "CarDNA.h"

#include <random>
#include <chrono>

const float CarDNA::minWidth = 1.0f;
const float CarDNA::minHeight = 1.0f;
const float CarDNA::minRadius = 1.0f;
const float CarDNA::widthMean = 10.0f;
const float CarDNA::widthVariance = 5.0f;
const float CarDNA::heightMean = 3.0f;
const float CarDNA::heightVariance = 1.0f;
const float CarDNA::radiusMean = 2.0f;
const float CarDNA::radiusVariance = 0.5f;
const float CarDNA::speedMean = 10.0f;
const float CarDNA::speedVariance = 5.0f;
const float CarDNA::mutationRate = 0.2f;

std::default_random_engine CarDNA::generator(std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<float> CarDNA::widthDist(CarDNA::widthMean, CarDNA::widthVariance);
std::normal_distribution<float> CarDNA::heightDist(CarDNA::heightMean, CarDNA::heightVariance);
std::normal_distribution<float> CarDNA::radiusDist(CarDNA::radiusMean, CarDNA::radiusVariance);
std::normal_distribution<float> CarDNA::speedDist(CarDNA::speedMean, CarDNA::speedVariance);
std::normal_distribution<float> CarDNA::mutationDist(1.0f, CarDNA::mutationRate);

CarDNA::CarDNA()
{
	width = randomWidth();
	leftHeight = randomHeight();
	rightHeight = randomHeight();
	leftRadius = randomRadius();
	rightRadius = randomRadius();
	leftSpeed = randomSpeed();
	rightSpeed = randomSpeed();
}

CarDNA::CarDNA(const CarDNA& other) {
	width = other.width;
	leftHeight = other.leftHeight;
	rightHeight = other.rightHeight;
	leftRadius = other.leftRadius;
	rightRadius = other.rightRadius;
	leftSpeed = other.leftSpeed;
	rightSpeed = other.rightSpeed;
}

float CarDNA::randomSelect(float a, float b) {
	std::uniform_int_distribution<int> eitherOr(0, 1);
	int select = eitherOr(generator);
	if (select == 0) {
		return a;
	}
	else {
		return b;
	}
}

CarDNA CarDNA::cross(const CarDNA& other) {
	CarDNA child;
	child.width = randomSelect(width, other.width) * mutationDist(generator);
	child.leftHeight = randomSelect(leftHeight, other.leftHeight) * mutationDist(generator);
	child.rightHeight = randomSelect(rightHeight, other.rightHeight) * mutationDist(generator);
	child.leftRadius = randomSelect(leftRadius, other.leftRadius) * mutationDist(generator);
	child.rightRadius = randomSelect(rightRadius, other.rightRadius) * mutationDist(generator);
	child.leftSpeed = randomSelect(leftSpeed, other.leftSpeed) * mutationDist(generator);
	child.rightSpeed = randomSelect(rightSpeed, other.rightSpeed) * mutationDist(generator);
	return child;
}

void CarDNA::mutate(float mutationRate) {
	std::uniform_real_distribution<float> select(0.0f, 1.0f);

	if (select(generator) <= mutationRate) {
		width = randomWidth();
	}
	if (select(generator) <= mutationRate) {
		leftHeight = randomHeight();
	}
	if (select(generator) <= mutationRate) {
		rightHeight = randomHeight();
	}
	if (select(generator) <= mutationRate) {
		leftRadius = randomRadius();
	}
	if (select(generator) <= mutationRate) {
		rightRadius = randomRadius();
	}
	if (select(generator) <= mutationRate) {
		leftSpeed = randomSpeed();
	}
	if (select(generator) <= mutationRate) {
		rightSpeed = randomSpeed();
	}
}


float CarDNA::randomWidth() {
	float w = widthDist(generator);
	if (w < minWidth) {
		w = minWidth;
	}
	return w;
}

float CarDNA::randomHeight() {
	float h = heightDist(generator);
	if (h < minHeight) {
		h = minHeight;
	}
	return h;
}

float CarDNA::randomRadius() {
	float r = radiusDist(generator);
	if (r < minRadius) {
		r = minRadius;
	}
	return r;
}

float CarDNA::randomSpeed() {
	return speedDist(generator);
}