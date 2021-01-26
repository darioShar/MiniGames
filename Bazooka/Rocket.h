#pragma once
#include "Entity.h"
#include <cmath>

#define DEG_TO_RAD 0.01745329251

class Rocket : public Entity
{
	float _speed;

public:
	Rocket(IntRect ground) {
		_name = "rocket";
	}
	~Rocket() {}

	ActionOnEntity update(float dt) {
		_x += _dx*dt;
		_y += _dy *dt;

		while (_x < 0) _x += WIDTH;
		while (_x > WIDTH) _x -= WIDTH;

		updateHitBox();
		_anim.update();
		if (_y > HEIGHT || _y < 0) _actionToDo = ActionOnEntity::RoundedExplosion;
		return _actionToDo;
	}

	string name() const {
		return _name;
	}

	void launch(float x, float y, float angle = 0.0f, float speed = 1.5f) {
		_speed = speed;
		_x = x; _y = y;
		_angle = 180.0f - angle;
		// convert to radian
		angle *= DEG_TO_RAD;
		_dx = cos(angle)* _speed;
		_dy = -sin(angle) * _speed;
	}
	
};