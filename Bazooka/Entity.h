#pragma once
#include "Animation.h"

enum ActionOnEntity {
	Nothing,
	Disappear,
	RoundedExplosion,
	NuclearExplosion,
	Die
};

class Entity {
protected :
	float _x, _y, _dx, _dy, _angle;
	string _name;
	Animation _anim;
	float _resizeFactor;
	IntRect _hitBox;

	ActionOnEntity _actionToDo;

	void updateHitBox() {
		_hitBox.left = _x - _hitBox.width / 2;
		_hitBox.top = _y - _hitBox.height / 2;
	}

public :
	Entity()  {}
	virtual ~Entity() {}

	void settings(Animation &a, int x = 0, int y = 0, float angle = 0.0f, Vector2u hitBox = Vector2u(15,15)) {
		_anim = a;
		_x = x; _y = y;
		_dx = 0.0f; _dy = 0.0f;
		_angle = angle;
		_resizeFactor = 1.0f;
		_actionToDo = ActionOnEntity::Nothing;

		// _x and _y parameters don't really matter.
		_hitBox = IntRect(_x, _y,hitBox.x, hitBox.y);
	}

	void draw(RenderWindow &window) {
		_anim._sprite.setPosition(_x, _y);
		_anim._sprite.setRotation(_angle);
		window.draw(_anim._sprite);

		// representing hit box;
		RectangleShape rec(Vector2f(_hitBox.width, _hitBox.height));
		rec.setFillColor(Color(255, 0, 0, 127));
		rec.setPosition(_hitBox.left, _hitBox.top);
		//window.draw(rec);
	}

	Vector2f getPosition() const {
		return Vector2f(_x, _y);
	}

	IntRect getHitBox() const {
		return _hitBox;
	}

	void changeAnimation(Animation &a) {
		_anim = a;
	}

	void setScale(float resizeFactor) {
		_hitBox.width *= resizeFactor / _resizeFactor;
		_hitBox.height *= resizeFactor / _resizeFactor;
		_resizeFactor = resizeFactor;
		_anim._sprite.setScale(resizeFactor, resizeFactor);
	}

	void setActionOnEntity(ActionOnEntity aoe) {
		_actionToDo = aoe;
	}

	bool collidesWith(const IntRect &e) const {
		return _hitBox.intersects(e);
	}

	virtual ActionOnEntity update(float dt) = 0;
	virtual string name() const = 0;
};