#pragma once
#include "Entity.h"

#define APPROX 1.0f

#define ON_AIR 0
#define ON_GROUND 1
#define NO_DIRECTION 0
#define DIRECTION 1 << 1
#define LEFT 0
#define RIGHT 1 << 2


enum PlayerMovement {
	MoveRight,
	MoveLeft,
	NoMove
};

enum PlayerState {
	OnAirRight,
	OnAirLeft,
	OnGroundStillRight,
	OnGroundStillLeft,
	OnGroundMovingRight,
	OnGroundMovingLeft
};

class Player : public Entity
{
	IntRect _ground;
	float _maxSpeed;
	float _gravity;
	int _actualState;

	inline void applyGravity(float dt) {
		if (!_ground.intersects(_hitBox)) {
			_y += _dy *dt;
			//if (_dy < _maxSpeed) {
				_y += _gravity*dt*dt / 2.0f;
				_dy += _gravity*dt;
			//}
			if (_y + _hitBox.height/2.0f > _ground.top) _y = _ground.top - _hitBox.height / 2.0f + 1; // + 1 so that we have the intersection
		}
	}

public:
	Player(float maxSpeed = 1.0f, float g = 0.006f, IntRect ground = IntRect(0, 5*HEIGHT / 9, WIDTH, 4*HEIGHT / 9)) {
		_name = "player";
		_maxSpeed = maxSpeed;
		_gravity = g;
		_ground = ground;
	}
	~Player() {}
	
	ActionOnEntity update(float dt) {
		// don't forget to apply gravity to player
		applyGravity(dt);

		// move
		_x += _dx * dt;
		if (_x < 0) _x = WIDTH; if (_x > WIDTH) _x = 0;

		updateHitBox();

		if (_ground.intersects(_hitBox)) _actualState |= ON_GROUND;
		// update animation
		_anim.update();
		return _actionToDo;
	}

	bool animationEnded() {
		return _anim.hasEnded();
	}
	
	string name() const {
		return _name;
	}

	void jump(float speed, float dt) {
		_dy = -speed;
		_y += _dy * dt;
		_actualState &= ~(ON_GROUND);
	}

	void move(const PlayerMovement &move, float dt) {
		if (move == PlayerMovement::MoveLeft) {
			_dx = -_maxSpeed;
			_actualState |= DIRECTION;
			_actualState &= ~(RIGHT);
		}
		else if (move == PlayerMovement::MoveRight) {
			_dx = _maxSpeed;
			_actualState |= DIRECTION | RIGHT;
		}
		else if (move == PlayerMovement::NoMove) {
			_dx = 0.0f;
			_actualState &= ~(DIRECTION);
		}

		//if (_ground.intersects(_hitBox)) _actualState |= ON_GROUND;
	}

	int getPlayerState() const {
		return _actualState;
	}
};