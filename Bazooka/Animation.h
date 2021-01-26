#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

constexpr int WIDTH = 1600;
constexpr int HEIGHT = 900;

using namespace sf;
using namespace std;

enum AnimationState : bool {
	CycleEnded,
	ThroughCycle
};

class Animation {
public :
	float _frame, _speed;
	Sprite _sprite;
	vector<IntRect> frames;
	float _width, _height;

	Animation() {}

	Animation(
		Texture &t,
		int x, int y, int w, int h,
		const Vector2i &spriteSheet2DCount,
		float speed,
		bool inverse)
	{
		_frame = 0;
		_speed = speed;
		_width = w; _height = h;

		int dir_y = spriteSheet2DCount.y > 0 ? 1 : -1;
		int dir_x = spriteSheet2DCount.x > 0 ? 1 : -1;
		for (int i = 0; i < abs(spriteSheet2DCount.y); i++)
			for (int j = 0; j < abs(spriteSheet2DCount.x); j++) {
				if (inverse) frames.push_back(IntRect(x + dir_x*j*w + w, y + dir_y*i*h, -w, h));
				else frames.push_back(IntRect(x + dir_x*j*w, y + dir_y*i*h, w, h));
			}
		_sprite.setTexture(t);
		_sprite.setOrigin(w / 2, h / 2);
		_sprite.setTextureRect(frames[0]);
	}

	void update()
	{
		_frame += _speed;
		int s = frames.size();
		if (_frame >= s) _frame -= s;
		if (s>0) _sprite.setTextureRect(frames[int(_frame)]);
	}

	bool hasEnded()
	{
		return (_frame + _speed >= frames.size());
	}
};