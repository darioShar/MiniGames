#pragma once
#include "Entity.h"

class Explosion : public Entity
{
public :
	Explosion() {
		_name = "explosion";
	}
	~Explosion() {}

	ActionOnEntity update(float dt) {
		updateHitBox();
		_anim.update();
		if(_anim.hasEnded()) _actionToDo = ActionOnEntity::Disappear;
		return _actionToDo;
	}

	string name() const {
		return _name;
	}
};