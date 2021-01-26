#include "Ball.h"



Ball::Ball(float radius, Color color)
	:	CircleShape(radius)
	,	isThrown(false)
{
	setFillColor(color);
	setOrigin(radius, radius);
}


Ball::~Ball()
{
}


void Ball::setSpeed(Vector2f speed) {
	m_speed = speed;
}

void Ball::setSpeed(float speed, float degree) {
	m_speed = Vector2f(speed * cosf(degree * M_PI / 180.0f), speed * sinf(degree * M_PI / 180.0f));
}

Vector2f Ball::getSpeed() {
	return m_speed;
}

float Ball::getNormSpeed() {
	return sqrtf(m_speed.x*m_speed.x + m_speed.y*m_speed.y);
}