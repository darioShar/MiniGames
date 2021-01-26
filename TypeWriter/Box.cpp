#include "Box.h"



Box::Box(int x, int y, int width, int height)
	:	m_xpos(x)
	,	m_ypos(y)
	,	m_width(width)
	,	m_height(height)
{
	m_font.loadFromFile(FONT_PATH);

	m_box.setPosition(Vector2f(x, y));
	m_box.setSize(Vector2f(width, height));
	m_box.setFillColor(Color::White);
	m_box.setOutlineThickness(5.0f);
	m_box.setOutlineColor(Color::White);

	m_text.setFont(m_font);
	m_text.setFillColor(Color::Black);
	m_text.setCharacterSize(INPUT_CHARACTER_SIZE);
	m_text.setOrigin(Vector2f(0.0f, INPUT_CHARACTER_SIZE / 2));
	m_text.setPosition(Vector2f(x + 10, y + height / 2));
}


Box::~Box()
{
}

void Box::setText(string const &str, Color const & col) {
	m_text.setString(str);
	m_text.setFillColor(col);
}

void Box::setPosition(int x, int y) {
	m_xpos = x;
	m_ypos = y;
	m_box.setPosition(Vector2f(x, y));
	m_text.setPosition(Vector2f(x + 10, y + m_height / 2));
}

void Box::setSize(int width, int height) {
	m_width = width;
	m_height = height;
	m_box.setSize(Vector2f(width, height));
	m_text.setPosition(Vector2f(m_xpos + 10, m_ypos + height / 2));
}

void Box::setOutlineSize(int outSize) {
	m_box.setOutlineThickness(outSize);
}

void Box::setColor(Color const &col) {
	m_box.setFillColor(col);
}

void Box::setOutlineColor(Color const &outCol) {
	m_box.setOutlineColor(outCol);
}

void Box::move(int x, int y) {
	m_text.move(x, y);
	m_box.move(x, y);
}

void Box::draw(RenderWindow &window) {
	window.draw(m_box);
	window.draw(m_text);
}
