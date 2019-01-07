#include "Stats.h"

#include <algorithm>

Stats::Stats(World& world, Population& population) :
	world(world),
	population(population)
{
}

void Stats::log() {
	data.push_back(getCurrentDataPoint());
}

inline float max(float a, float b) {
	return (a > b) ? a : b;
}

inline float min(float a, float b) {
	return (a < b) ? a : b;
}

void Stats::drawDataPoint(int gen, DataPoint dataPoint, sf::RenderTarget &target, sf::RenderStates states) const {
	const float carAreaWidth = 100.0f;

	int numGens = data.size() + 1;
	float height = (float) target.getSize().y / (float) numGens;
	float scaleX = ((float) target.getSize().x - 100.0f) / world.getWidth();

	// draw the best car on the left
	float totalCarWidth = dataPoint.bestDNA.width + dataPoint.bestDNA.leftRadius + dataPoint.bestDNA.rightRadius;
	float totalLeftHeight = max(dataPoint.bestDNA.leftRadius + dataPoint.bestDNA.leftHeight, dataPoint.bestDNA.leftRadius * 2);
	float totalRightHeight = max(dataPoint.bestDNA.rightRadius + dataPoint.bestDNA.rightHeight, dataPoint.bestDNA.rightRadius * 2);
	float totalCarHeight = max(totalLeftHeight, totalRightHeight);

	float carScaleW = carAreaWidth / totalCarWidth;
	float carScaleH = height / totalCarHeight;
	float carScale = min(carScaleW, carScaleH);

	states.transform.translate(dataPoint.bestDNA.leftRadius * carScale, gen * height + height / 2.0f);
	states.transform.scale(carScale, -carScale);
	CarGraphics carGraphics(dataPoint.bestDNA);
	target.draw(carGraphics, states);
	
	// draw the box plot

	// the min and max lines
	float width = dataPoint.bestDistance - dataPoint.worstDistance;

	sf::RectangleShape minLine(sf::Vector2f(1.0, height / 2.0f));
	minLine.setFillColor(sf::Color::Black);
	minLine.setPosition(carAreaWidth + dataPoint.worstDistance * scaleX, gen * height + height / 4.0f);
	target.draw(minLine);

	sf::RectangleShape maxLine(sf::Vector2f(1.0, height / 2.0f));
	maxLine.setFillColor(sf::Color::Black);
	maxLine.setPosition(carAreaWidth + dataPoint.bestDistance * scaleX, gen * height + height / 4.0f);
	target.draw(maxLine);

	sf::RectangleShape minmaxLine(sf::Vector2f(width*scaleX, 1.0f));
	minmaxLine.setFillColor(sf::Color::Black);
	minmaxLine.setPosition(carAreaWidth + dataPoint.worstDistance * scaleX, gen * height + height / 2.0f);
	target.draw(minmaxLine);

	// the box
	float percWidth = dataPoint.upperPercentile - dataPoint.lowerPercentile;
	sf::RectangleShape percs(sf::Vector2f(percWidth*scaleX, height * 0.8));
	percs.setOutlineThickness(1.0f);
	percs.setOutlineColor(sf::Color::Black);
	percs.setFillColor(sf::Color(255, 255, 255, 200));
	percs.setPosition(carAreaWidth + dataPoint.lowerPercentile * scaleX, gen * height + height / 10.0f);
	target.draw(percs);

	// the mean
	sf::RectangleShape mean(sf::Vector2f(1.0f, height * 0.8));
	mean.setFillColor(sf::Color::Black);
	mean.setPosition(carAreaWidth + dataPoint.meanDistance * scaleX, gen * height + height / 10.0f);
	target.draw(mean);
}

void Stats::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	drawTerrain(target, states);
	for (int i = 0; i < data.size(); i++) {
		drawDataPoint(i, data[i], target, states);
	}
	drawDataPoint(data.size(), getCurrentDataPoint(), target, states);
}

DataPoint Stats::getCurrentDataPoint() const {
	// sort the cars by their fitness
	std::vector<Car> cars = population.getCars();
	std::sort(cars.begin(), cars.end());

	float distanceSum = 0.0f;
	for (int i = 0; i < cars.size(); i++) {
		distanceSum += cars[i].getBestDistance();
	}

	// create a new data point according to the population
	DataPoint data = {
		cars[cars.size() - 1].getDNA(),
		cars[cars.size() - 1].getBestDistance(),
		cars[0].getBestDistance(),
		cars[cars.size()/2].getBestDistance(),
		cars[(int)(cars.size()*0.75)].getBestDistance(),
		cars[(int)(cars.size()*0.25)].getBestDistance(),
		distanceSum / (float) cars.size()
	};
	return data;
}


void Stats::drawTerrain(sf::RenderTarget &target, sf::RenderStates states) const {
	float scaleX = ((float)target.getSize().x-100.0f) / world.getWidth();
	states.transform.translate(100.0f, 0.0f);
	states.transform.translate(0.0f, (float)target.getSize().y );
	states.transform.scale(scaleX, -scaleX);
	target.draw(world, states);
}