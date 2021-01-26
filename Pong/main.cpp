#include <SFML\Graphics.hpp>
#include <Windows.h>
#include "Board.h"
#include "Pad.h"

#define WIDTH 1600
#define HEIGHT 900

#define PIXEL_PER_SECONDS_UP 1500.0f
#define PIXEL_PER_SECONDS_DOWN 1500.0f

#define BALL_SPEED 1250.0f

using namespace sf;
using namespace std;

int main() {
	Board board(VideoMode(WIDTH, HEIGHT), "Pong");
	Event evnt;

	Clock clock;
	float time = 0.0f;

	while (board.isOpen()) {
		while (board.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				board.close();
			}
		}

		time = clock.getElapsedTime().asSeconds();
		if (Keyboard::isKeyPressed(Keyboard::Z)) {
			board.moveLeftPad(-PIXEL_PER_SECONDS_UP * time);
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			board.moveLeftPad(PIXEL_PER_SECONDS_DOWN * time);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			board.moveRightPad(-PIXEL_PER_SECONDS_UP * time);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			board.moveRightPad(PIXEL_PER_SECONDS_DOWN * time);
		}
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			board.throwBall(BALL_SPEED, (180*(rand()%2)) + ((rand() % 90) - 45));
		}

		board.calculateNextPosition(time);

		clock.restart();
		board.clear();
		board.drawBoard();
		board.display();
	}
}