#pragma once
#include <SFML\Graphics.hpp>
#include "Pad.h"
#include "Ball.h"

using namespace sf;
using namespace std;


class Board : public RenderWindow
{
public:
	Board(VideoMode, string);
	~Board();

	void resetScores();
	void addPoint(PadType);

	void drawBoard();

	void moveLeftPad(float);
	void moveRightPad(float);

	void resetBallPos();
	void throwBall(float, float);
	void throwBall(Vector2f);

	void calculateNextPosition(float time);

private :
	Font m_font;
	Text m_title;
	pair<int, int> m_scores;
	pair<Text, Text> m_scoresText;

	RectangleShape m_midLine;

	Pad m_leftPad;
	Pad m_rightPad;

	Ball m_ball;
};

