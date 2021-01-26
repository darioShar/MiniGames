#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;

class Box
{
	float m_xpos, m_ypos;
	float m_width, m_height;
	Font m_font;
	Text m_text;
	RectangleShape m_box;

public:
	Box(float x, float y, float width, float height, Uint32 characterSize = 32);

	void setText(std::string const &, Color const & = Color::White);
	void setPosition(float, float);
	void setSize(float, float);
	void setOutlineSize(float);
	void setColor(Color const &);
	void setOutlineColor(Color const &);

	Vector2f getSize() const { return Vector2f(m_width, m_height); }

	void move(float x, float y);
	void draw(RenderWindow &);
};