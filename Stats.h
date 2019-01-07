#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Population.h"

/*
One data-point that is stored in the statistics (for one generation)
*/
struct DataPoint {
	CarDNA bestDNA;
	float bestDistance;
	float worstDistance;
	float meanDistance;
	float upperPercentile;
	float lowerPercentile;
	float averageDistance;
};

/*
Statistics about every generation.
*/
class Stats :
	public sf::Drawable
{
private:
	World& world;
	Population& population;
	std::vector<DataPoint> data;
public:
	Stats(World& world, Population& population);
	// logs the current population's status
	void log();
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	DataPoint getCurrentDataPoint() const;
	void drawDataPoint(int gen, DataPoint data, sf::RenderTarget &target, sf::RenderStates states) const;
	void drawTerrain(sf::RenderTarget &target, sf::RenderStates states) const;
};

