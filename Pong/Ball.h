#pragma once
#include <SFML\Graphics.hpp>
#include <cmath>
#define M_PI 3.14159265358979323846

using namespace sf;

class Ball : public CircleShape
{
public:
	Ball(float radius = 10.0f, Color color = Color::White);
	~Ball();

	void setSpeed(Vector2f speed);
	void setSpeed(float speed, float degree);
	Vector2f getSpeed();
	float getNormSpeed();
	bool isThrown;
private :
	Vector2f m_speed;
};

