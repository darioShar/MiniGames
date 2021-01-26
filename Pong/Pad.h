#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

enum class PadType : bool {
	Left,
	Right
};

class Pad : public RectangleShape
{
public:
	Pad(int windowWidth = 1600, int windowHeight = 900, PadType LeftOrRight = PadType::Left, Color color = Color::White);
	~Pad();

	void movePad(float offsetY);

private :
	int m_windowWidth;
	int m_windowHeight;
};

