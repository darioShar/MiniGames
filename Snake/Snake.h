#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;

class Snake
{
	int						m_bodySize;
	int						m_direction;
	Vector2u				m_screenSize;
	RectangleShape			m_bodyRect;
	std::vector<Vector2f>	m_bodiesPos;

public:
	Snake(Vector2u &screenSize, int size = 25)
		: m_screenSize(screenSize)
		, m_bodySize(size)
		, m_direction(0x00000001)
	{
		m_bodyRect.setFillColor(Color::Green);
		m_bodyRect.setSize(Vector2f(m_bodySize, m_bodySize));
		m_bodiesPos.push_back(Vector2f());
	}

	bool headCollides(FloatRect& rect) { return rect.intersects(FloatRect(m_bodiesPos[0], Vector2f(m_bodySize, m_bodySize))); }

	bool move() {
		for (int i = (int)m_bodiesPos.size() - 1; i > 0; i--) m_bodiesPos[i] = m_bodiesPos[i - 1]; // Move bodies
		if (m_direction & 1) m_bodiesPos[0].x += m_direction * m_bodySize; // Move head
		else m_bodiesPos[0].y += (m_direction + 1) * m_bodySize;

		if (m_bodiesPos[0].x >= m_screenSize.x) m_bodiesPos[0].x = 0.0f;
		else if (m_bodiesPos[0].x < 0.0f) m_bodiesPos[0].x = m_screenSize.x - m_bodySize;
		if (m_bodiesPos[0].y >= m_screenSize.y) m_bodiesPos[0].y = 0.0f;
		else if (m_bodiesPos[0].y < 0.0f) m_bodiesPos[0].y = m_screenSize.y - m_bodySize;

		for (int i = 1; i < m_bodiesPos.size(); i++) if (m_bodiesPos[i] == m_bodiesPos[0]) return false; // Check if auto-touches
		return true;
	}

	void addBody() {
		m_bodiesPos.push_back(Vector2f());
		move();
	}

	void goDown()	{ if (m_direction != 0xfffffffe) m_direction = 0x00000000; }
	void goUp()		{ if (m_direction != 0x00000000) m_direction = 0xfffffffe; }
	void goLeft()	{ if (m_direction != 0x00000001) m_direction = 0xffffffff; }
	void goRight()	{ if (m_direction != 0xffffffff) m_direction = 0x00000001; }

	void draw(RenderWindow& window) {
		for (auto &bodyPos : m_bodiesPos) {
			m_bodyRect.setPosition(bodyPos);
			window.draw(m_bodyRect);
		}
	}
};