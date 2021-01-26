#include "Pad.h"

#define PAD_WIDTH_REDUCTION 80.0f
#define PAD_HEIGHT_REDUCTION 5.0f


Pad::Pad(int windowWidth, int windowHeight, PadType LeftOrRight, Color color)
	:	m_windowWidth(windowWidth)
	,	m_windowHeight(windowHeight)
{
	setSize(Vector2f((float)windowWidth / PAD_WIDTH_REDUCTION, (float)windowHeight / PAD_HEIGHT_REDUCTION));
	setFillColor(color);
	if (LeftOrRight == PadType::Left) {
		setOrigin((float)windowWidth / PAD_WIDTH_REDUCTION, 0.0f);
		move(4.0f * ((float)windowWidth / PAD_WIDTH_REDUCTION), (float)windowHeight / 2.0f);
	} 
	else {
		move((float)windowWidth - (3.0f * (float)windowWidth / PAD_WIDTH_REDUCTION), (float)windowHeight / 2.0f);
	}
}

Pad::~Pad()
{
}

void Pad::movePad(float offsetY)
{
	float YposUp = getPosition().y + offsetY;
	float YposDown = getPosition().y + offsetY + getSize().y;

	if (YposUp > 0.0f && YposDown < m_windowHeight) {
		move(0.0f, offsetY);
	}
}
