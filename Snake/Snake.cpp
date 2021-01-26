#include "Snake.h"

Snake::Snake(Vector2u &screenSize, int size)
	:	m_screenSize(screenSize)
	,	m_bodySize(size)
	,	m_direction(0x00000001)
{
	m_bodyRect.setFillColor(Color::Green);
	m_bodyRect.setSize(Vector2f(m_bodySize, m_bodySize));
	m_bodiesPos.push_back(Vector2f());
}

bool Snake::move() {
	for (int i = m_bodiesPos.size() - 1; i > 0; i--) m_bodiesPos[i] = m_bodiesPos[i - 1]; // Move bodies
	m_bodiesPos[0] += Vector2f(m_bodySize * (m_direction & 1 ? m_direction : 0), m_bodySize * (m_direction & 1 ? 0 : m_direction + 1)); // Move head
	m_bodiesPos[0].x = fmodf(fmodf(m_bodiesPos[0].x, m_screenSize.x) + m_screenSize.x, m_screenSize.x);
	m_bodiesPos[0].y = fmodf(fmodf(m_bodiesPos[0].y, m_screenSize.y) + m_screenSize.y, m_screenSize.y);
	for (int i = 3; i < m_bodiesPos.size(); i++) if (m_bodiesPos[i] == m_bodiesPos[0]) return false; // Check if auto-touches
	return true;
}

void Snake::addBody() {
	m_bodiesPos.push_back(Vector2f());
	move();
}

void Snake::goDown()	{ m_direction = 0x00000000; }
void Snake::goUp()		{ m_direction = 0xfffffffe; }
void Snake::goLeft()	{ m_direction = 0xffffffff; }
void Snake::goRight()	{ m_direction = 0x00000001; }

void Snake::draw(RenderWindow& window) {
	window.clear();
	for (auto &bodyPos : m_bodiesPos) {
		m_bodyRect.setPosition(bodyPos);
		window.draw(m_bodyRect);
	}
	window.display();
}