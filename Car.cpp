#include "Car.h"

const float Car::CAR_DENSITY = 1.0f;
const float Car::CAR_FRICTION = 1.0f;
const float Car::CAR_RESTITUTION = 0.1f;

const float Car::WHEEL_DENSITY = 1.0f;
const float Car::WHEEL_FRICTION = 10.0f;
const float Car::WHEEL_RESTITUTION = 0.1f;
const float Car::WHEEL_MOTOR_MAXTORQUE = 10000;

const float Car::TIME_TO_DEATH = 5.0f;

Car::Car(CarDNA dna, b2Vec2 startPos, float endDistance, b2World& world)
	: dna(dna),
	world(world),
	bestDistance(startPos.x),
	timeSinceLastImprovement(0),
	timeToBestDistance(0),
	timeAlive(0),
	endDistance(endDistance),
	graphics(CarGraphics(dna))
{
	initBody(startPos);
	initWheels(startPos);
}

void Car::initBody(b2Vec2 pos) {
	b2BodyDef carBodyDef;
	carBodyDef.type = b2_dynamicBody;
	carBodyDef.position = pos;
	carBodyDef.angle = 0;
	carBody = world.CreateBody(&carBodyDef);

	b2Vec2 verts[4];
	verts[0].Set(0, 0);
	verts[1].Set(0, dna.leftHeight);
	verts[2].Set(dna.width, dna.rightHeight);
	verts[3].Set(dna.width, 0);

	b2PolygonShape carShape;
	carShape.Set(verts, 4);

	b2FixtureDef carFixtureDef;
	carFixtureDef.density = CAR_DENSITY;
	carFixtureDef.friction = CAR_FRICTION;
	carFixtureDef.restitution = CAR_RESTITUTION;
	carFixtureDef.filter.groupIndex = -1;
	carFixtureDef.shape = &carShape;

	carBody->CreateFixture(&carFixtureDef);
}

void Car::initWheels(b2Vec2 pos) {
	// rear wheel
	b2CircleShape rearWheelShape;
	rearWheelShape.m_radius = dna.leftRadius;

	b2FixtureDef rearWheelFixDef;
	rearWheelFixDef.density = WHEEL_DENSITY;
	rearWheelFixDef.friction = WHEEL_FRICTION;
	rearWheelFixDef.restitution = WHEEL_RESTITUTION;
	rearWheelFixDef.filter.groupIndex = -1;
	rearWheelFixDef.shape = &rearWheelShape;

	b2BodyDef rearWheelBodyDef;
	rearWheelBodyDef.type = b2_dynamicBody;
	rearWheelBodyDef.position = pos;
	rearWheelBody = world.CreateBody(&rearWheelBodyDef);
	rearWheelBody->CreateFixture(&rearWheelFixDef);

	b2RevoluteJointDef rearJointDef;
	rearJointDef.Initialize(rearWheelBody, carBody, rearWheelBody->GetWorldCenter());
	rearJointDef.enableMotor = true;
	rearJointDef.maxMotorTorque = WHEEL_MOTOR_MAXTORQUE;
	rearWheelJoint = (b2RevoluteJoint*) world.CreateJoint(&rearJointDef);
	rearWheelJoint->SetMotorSpeed(dna.leftSpeed);

	// front wheel
	b2CircleShape frontWheelShape;
	frontWheelShape.m_radius = dna.rightRadius;

	b2FixtureDef frontWheelFixDef;
	frontWheelFixDef.density = WHEEL_DENSITY;
	frontWheelFixDef.friction = WHEEL_FRICTION;
	frontWheelFixDef.restitution = WHEEL_RESTITUTION;
	frontWheelFixDef.filter.groupIndex = -1;
	frontWheelFixDef.shape = &frontWheelShape;

	b2BodyDef frontWheelBodyDef;
	frontWheelBodyDef.type = b2_dynamicBody;
	frontWheelBodyDef.position.Set(pos.x + dna.width, pos.y);
	frontWheelBody = world.CreateBody(&frontWheelBodyDef);
	frontWheelBody->CreateFixture(&frontWheelFixDef);

	b2RevoluteJointDef frontJointDef;
	frontJointDef.Initialize(frontWheelBody, carBody, frontWheelBody->GetWorldCenter());
	frontJointDef.enableMotor = true;
	frontJointDef.maxMotorTorque = WHEEL_MOTOR_MAXTORQUE;
	frontWheelJoint = (b2RevoluteJoint*) world.CreateJoint(&frontJointDef);
	frontWheelJoint->SetMotorSpeed(dna.rightSpeed);
}

void Car::update(float elapsed) {
	if (isDead() && getPosition().x > endDistance) {
		return;
	}

	timeAlive += elapsed;
	timeSinceLastImprovement += elapsed;

	if (getPosition().x > bestDistance) {
		bestDistance = getPosition().x;
		timeToBestDistance = timeAlive;
		timeSinceLastImprovement = 0;
	}

	graphics.update(rearWheelBody->GetAngle() * 180.0f / 3.1415f,
		frontWheelBody->GetAngle() * 180.0f / 3.1415f,
		isDead());
}

void Car::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	states.transform.translate(carBody->GetPosition().x, carBody->GetPosition().y);
	states.transform.rotate(carBody->GetAngle() * 180.0f / 3.1415f);
	target.draw(graphics, states);
}

sf::Vector2f Car::getPosition() const {
	return sf::Vector2f(carBody->GetPosition().x, carBody->GetPosition().y);
}

bool Car::isDead() const {
	return (timeSinceLastImprovement >= TIME_TO_DEATH || getPosition().x >= endDistance);
}

float Car::getFitness() const {
	if (getPosition().x >= endDistance) {
		return (bestDistance - timeToBestDistance)*10.0f;
	}
	else {
		return bestDistance - timeToBestDistance;
	}
}

CarDNA Car::getDNA() const {
	return dna;
}

void Car::destroy() {
	world.DestroyJoint(rearWheelJoint);
	world.DestroyJoint(frontWheelJoint);
	world.DestroyBody(rearWheelBody);
	world.DestroyBody(frontWheelBody);
	world.DestroyBody(carBody);
}

float Car::getBestDistance() const {
	return bestDistance;
}

bool Car::operator <(const Car& other) const {
	return bestDistance < other.bestDistance;
}

Car& Car::operator =(const Car& other) {
	dna = other.dna;
	carBody = other.carBody;
	rearWheelBody = other.rearWheelBody;
	rearWheelJoint = other.rearWheelJoint;
	frontWheelBody = other.frontWheelBody;
	frontWheelJoint = other.frontWheelJoint;

	graphics = other.graphics;

	bestDistance = other.bestDistance;
	timeToBestDistance = other.timeToBestDistance;
	timeSinceLastImprovement = other.timeSinceLastImprovement;
	timeAlive = other.timeAlive;

	endDistance = other.endDistance;

	return *this;
}

CarGraphics::CarGraphics(CarDNA dna)
	: carShape(sf::ConvexShape(4))
{
	initGraphics(dna);
}

void CarGraphics::initGraphics(CarDNA dna) {
	carShape.setPoint(0, sf::Vector2f(0, 0));
	carShape.setPoint(1, sf::Vector2f(0, dna.leftHeight));
	carShape.setPoint(2, sf::Vector2f(dna.width, dna.rightHeight));
	carShape.setPoint(3, sf::Vector2f(dna.width, 0));
	carShape.setFillColor(sf::Color::White);
	carShape.setOutlineThickness(0.1f);
	carShape.setOutlineColor(sf::Color::Black);

	rearWheelShape.setRadius(dna.leftRadius);
	rearWheelShape.setOrigin(dna.leftRadius, dna.leftRadius);
	rearWheelShape.setPosition(0, 0);
	rearWheelShape.setFillColor(sf::Color::White);
	rearWheelShape.setOutlineThickness(0.1f);
	rearWheelShape.setOutlineColor(sf::Color::Black);

	frontWheelShape.setRadius(dna.rightRadius);
	frontWheelShape.setOrigin(dna.rightRadius, dna.rightRadius);
	frontWheelShape.setPosition(dna.width, 0);
	frontWheelShape.setFillColor(sf::Color::White);
	frontWheelShape.setOutlineThickness(0.1f);
	frontWheelShape.setOutlineColor(sf::Color::Black);

	rearWheelLine = sf::RectangleShape(sf::Vector2f(dna.leftRadius * 2, 0.1f));
	rearWheelLine.setFillColor(sf::Color::Black);
	rearWheelLine.setPosition(0.0f, 0.0f);
	rearWheelLine.setOrigin(dna.leftRadius, 0.0f);

	frontWheelLine = sf::RectangleShape(sf::Vector2f(dna.rightRadius * 2, 0.1f));
	frontWheelLine.setFillColor(sf::Color::Black);
	frontWheelLine.setPosition(dna.width, 0.0f);
	frontWheelLine.setOrigin(dna.rightRadius, 0.0f);
}

void CarGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(carShape, states);
	target.draw(rearWheelShape, states);
	target.draw(rearWheelLine, states);
	target.draw(frontWheelShape, states);
	target.draw(frontWheelLine, states);
}


void CarGraphics::update(float rearRot, float frontRot, bool isDead) {
	rearWheelLine.setRotation(rearRot);
	frontWheelLine.setRotation(frontRot);

	if (isDead) {
		carShape.setFillColor(sf::Color::Red);
	}
	else {
		carShape.setFillColor(sf::Color::White);
	}
}