#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include "filepaths.h"


#define INPUT_CHARACTER_SIZE 32


using namespace sf;
using namespace std;

class Box
{
	// Position and size
	int m_xpos, m_ypos;
	int m_width, m_height;

	Font m_font;
	Text m_text;
	RectangleShape m_box;

public:
	Box(int, int, int, int);
	~Box();

	void setText(string const &, Color const & = Color::Black);
	void setPosition(int, int);
	void setSize(int, int);
	void setOutlineSize(int);
	void setColor(Color const &);
	void setOutlineColor(Color const &);

	void move(int x, int y);

	void draw(RenderWindow &);
};

