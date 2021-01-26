#include "Box.h"

Box::Box(float x, float y, float width, float height, Uint32 characterSize)
	:	m_xpos(x)
	,	m_ypos(y)
	,	m_width(width)
	,	m_height(height)
{
	m_font.loadFromFile("C:\\Windows\\Fonts\\ariblk.ttf");

	m_box.setFillColor(Color::Transparent);
	m_box.setOutlineThickness(5.0f);
	m_box.setOutlineColor(Color::Transparent);

	m_text.setFont(m_font);
	m_text.setFillColor(Color::White);
	m_text.setCharacterSize(characterSize);
	m_text.setOrigin(Vector2f(0.0f, (float)characterSize / 2));

	setPosition(m_xpos, m_ypos);
	setSize(m_width, m_height);
}

void Box::setText(std::string const &str, Color const & col) {
	m_text.setString(str);
	m_text.setFillColor(col);
	m_text.setPosition(m_xpos + (m_width - m_text.getLocalBounds().width) / 2.0f, m_ypos + (m_height - m_text.getLocalBounds().height) / 2.0f);
}

void Box::setPosition(float x, float y) {
	m_xpos = x;
	m_ypos = y;
	m_box.setPosition(Vector2f(x, y));
	m_text.setPosition(m_xpos + (m_width - m_text.getLocalBounds().width) / 2.0f, m_ypos + (m_height - m_text.getLocalBounds().height) / 2.0f);
}

void Box::setSize(float width, float height) {
	m_width = width;
	m_height = height;
	m_box.setSize(Vector2f(width, height));
	m_text.setPosition(m_xpos + (m_width - m_text.getLocalBounds().width) / 2.0f, m_ypos + (m_height - m_text.getLocalBounds().height) / 2.0f);
}

void Box::setOutlineSize(float outSize) { m_box.setOutlineThickness(outSize); }
void Box::setColor(Color const &col) { m_box.setFillColor(col); }
void Box::setOutlineColor(Color const &outCol) { m_box.setOutlineColor(outCol); }

void Box::move(float x, float y) {
	m_text.move(x, y);
	m_box.move(x, y);
}

void Box::draw(RenderWindow &window) {
	window.draw(m_box);
	window.draw(m_text);
}