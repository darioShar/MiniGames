#pragma once
#include <SFML\Graphics.hpp>

#define DEG_TO_RAD 0.01745329251f

using namespace sf;
using namespace std;

template <int T> struct Particles {
	Vector2f *speed;
	Vector2f *position;
	Vertex *points;
	float _time;
	float _lifeTime;

	Particles(float x, float y, float maxInitialSpeed, float lifeTime = 500.0f) {
		_time = 0.0f;
		_lifeTime = lifeTime;
		position = new(nothrow) Vector2f[T];
		speed = new(nothrow) Vector2f[T];
		points = new(nothrow) Vertex[T];

		if (position == nullptr || speed == nullptr || points == nullptr) {
			cout << "Particles Object allocation failed" << endl;
			delete this;
		}
		for (int i = 0; i < T; i++) {
			position[i] = Vector2f(x, y);
			float angle = rand() % 360;
			float initspeed = fmodf(maxInitialSpeed * (float)rand() / (float)RAND_MAX, maxInitialSpeed);
			speed[i] = Vector2f(cosf(angle * DEG_TO_RAD) * initspeed, sinf(angle * DEG_TO_RAD) * initspeed);
			points[i] = Vertex(position[i], Color(255, 255, 0));
		}
	}
	~Particles() {
		delete[] position;
		delete[] speed;
		delete[] points;
	}
};