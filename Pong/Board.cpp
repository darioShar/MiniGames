#include "Board.h"

#define MIDLINE_WIDTH 5.0f

Board::Board(VideoMode v, string s) 
	:	RenderWindow(v, s)
{
	m_midLine.setSize(Vector2f(MIDLINE_WIDTH, (float)v.height));
	m_midLine.move((v.width - MIDLINE_WIDTH) / 2.0f, 90.0f);

	m_font.loadFromFile("ROCK.ttf");
	// Create a text
	m_title.setFont(m_font);
	m_title.setString("PONG");
	m_title.setCharacterSize(72);
	m_title.setStyle(Text::Bold);
	m_title.setFillColor(Color::White);
	m_title.move((v.width - m_title.getGlobalBounds().width) / 2.0f, 0.0f);

	m_scores.first = 0;
	m_scores.second = 0;

	m_scoresText.first.setString(to_string(m_scores.first));
	m_scoresText.first.setFont(m_font);
	m_scoresText.first.setCharacterSize(45);
	m_scoresText.first.setStyle(Text::Bold);
	m_scoresText.first.setFillColor(Color::White);
	m_scoresText.first.move(m_scoresText.second.getCharacterSize()*2.0f, v.height / 20.0f);

	m_scoresText.second.setString(to_string(m_scores.second));
	m_scoresText.second.setFont(m_font);
	m_scoresText.second.setCharacterSize(45);
	m_scoresText.second.setStyle(Text::Bold);
	m_scoresText.second.setFillColor(Color::White);
	m_scoresText.second.move(v.width - (m_scoresText.second.getCharacterSize()*2.0f), v.height / 20.0f);

	m_leftPad = Pad(v.width, v.height, PadType::Left);
	m_rightPad = Pad(v.width, v.height, PadType::Right);

	resetBallPos();
}


Board::~Board()
{
}

void Board::resetScores()
{
	m_scores.first = 0;
	m_scores.second = 0;
	m_scoresText.first.setString(to_string(m_scores.first));
	m_scoresText.second.setString(to_string(m_scores.second));
}

void Board::addPoint(PadType pad)
{
	if (pad == PadType::Left) {
		m_scoresText.first.setString(to_string(++m_scores.first));
	}
	else {
		m_scoresText.second.setString(to_string(++m_scores.second));
	}
}


void Board::drawBoard() {
	draw(m_title);
	draw(m_scoresText.first);
	draw(m_scoresText.second);
	draw(m_midLine);
	draw(m_leftPad);
	draw(m_rightPad);
	draw(m_ball);
}


void Board::moveLeftPad(float offset) {
	m_leftPad.movePad(offset);
}

void Board::moveRightPad(float offset) {
	m_rightPad.movePad(offset);
}

void Board::resetBallPos() {
	m_ball.setPosition(getSize().x / 2.0f, getSize().y / 2.0f);
	m_ball.isThrown = false;
}

void Board::throwBall(float speed, float degree) {
	if (m_ball.isThrown == false) {
		m_ball.setSpeed(speed, degree);
	}
	m_ball.isThrown = true;
}

void Board::throwBall(Vector2f speed) {
	if (m_ball.isThrown == false) {
		m_ball.setSpeed(speed);
	}
	m_ball.isThrown = true;
}

void Board::calculateNextPosition(float time)
{
	if (m_ball.isThrown) {
		Vector2f newBallPos = m_ball.getPosition() + (time * m_ball.getSpeed());
		if (newBallPos.x < 0.0f) {
			addPoint(PadType::Right);
			resetBallPos();
		}
		else if (newBallPos.x > getSize().x) {
			addPoint(PadType::Left);
			resetBallPos();
		}
		else {
			if (newBallPos.y < 0.0f || newBallPos.y > getSize().y) {
				m_ball.setSpeed(Vector2f(m_ball.getSpeed().x, -m_ball.getSpeed().y));
			}

			if (abs(newBallPos.x - m_leftPad.getPosition().x) < 2.0f && ((newBallPos.y - m_leftPad.getPosition().y < m_leftPad.getSize().y) && (newBallPos.y - m_leftPad.getPosition().y > 0.0f))) {
				m_ball.setSpeed(m_ball.getNormSpeed(), -(((newBallPos.y - m_leftPad.getPosition().y) / m_leftPad.getSize().y) - 0.5f)* 180);
			}
			else if (abs(newBallPos.x - m_rightPad.getPosition().x) < 2.0f && ((newBallPos.y - m_rightPad.getPosition().y < m_rightPad.getSize().y) && (newBallPos.y - m_rightPad.getPosition().y > 0.0f))) {
				m_ball.setSpeed(m_ball.getNormSpeed(), 180 + ((((newBallPos.y - m_rightPad.getPosition().y) / m_rightPad.getSize().y) - 0.5f) * 180));
			}

			m_ball.move(time * m_ball.getSpeed());
		}
	}
}
