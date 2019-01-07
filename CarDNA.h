#pragma once

#include <random>

/*
The DNA of a car.
Contains every bit of information that is needed to generate a car.
*/
class CarDNA
{
private:
	static std::default_random_engine generator;
	static std::normal_distribution<float> widthDist;
	static std::normal_distribution<float> heightDist;
	static std::normal_distribution<float> radiusDist;
	static std::normal_distribution<float> speedDist;
	static std::normal_distribution<float> mutationDist;
	static const float minWidth;
	static const float minHeight;
	static const float minRadius;
	static const float widthMean;
	static const float widthVariance;
	static const float heightMean;
	static const float heightVariance;
	static const float radiusMean;
	static const float radiusVariance;
	static const float speedMean;
	static const float speedVariance;
	static const float mutationRate;

public:
	float width;
	float leftHeight;
	float rightHeight;
	float leftRadius;
	float rightRadius;
	float leftSpeed;
	float rightSpeed;

	CarDNA();
	CarDNA(const CarDNA& other);
	CarDNA cross(const CarDNA& other);
	void mutate(float mutationRate);
private:
	float randomSelect(float a, float b);
	static float randomWidth();
	static float randomHeight();
	static float randomRadius();
	static float randomSpeed();
};