#include <SFML\Graphics.hpp>
#include "FlyingText.h"
#include "Box.h"

#define WIDTH 1600
#define HEIGHT 900

#define TEXT_INITIAL_SPEED 0.042f
#define TEXT_ACCELERATION_FACTOR 1.3f

#define TEXT_INITIAL_WORDS 20
#define TEXT_MULTIPLICATION_FACTOR 1.2


#define NUM_LEVEL 5

using namespace sf;

int main() {
	srand(time(0));
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "TypeWriter");
	window.setFramerateLimit(60);
	Event evnt;
	Clock clock;
	float dt = 0.00000001f;

	FlyingText flyingText(WIDTH, HEIGHT, TEXT_INITIAL_SPEED, TEXT_INITIAL_WORDS);
	string inputText("");
	Color inputTextColor;
	Box inputBox(3 * WIDTH / 4, 8 * HEIGHT / 9, WIDTH / 4 - 15, HEIGHT / 9 - 10);
	inputBox.setColor(Color(9, 226, 194));
	inputBox.setOutlineColor(Color::White);

	int scoresBetweenLevelUp[] = { 2000, 5000, 10000, 20000 };
	if(sizeof(scoresBetweenLevelUp) / sizeof(int) < NUM_LEVEL - 1) return 1;
	int score = 0;
	Box scoreBox(8 * WIDTH / 10, HEIGHT / 100, 2 * WIDTH / 10 - 20, HEIGHT / 12);
	scoreBox.setColor(Color::Black);
	scoreBox.setOutlineColor(Color::Black);
	scoreBox.setText("Score : " + to_string(score), Color::Red);

	int level = 0;
	Box scoreLevel(17 * WIDTH / 20, HEIGHT / 12 + 10, WIDTH / 10 - 30, HEIGHT / 12);
	scoreLevel.setColor(Color::Black);
	scoreLevel.setOutlineColor(Color::Black);
	scoreLevel.setText("Level " + to_string(level), gameScores[level].color);

	while (window.isOpen()) {
		while (window.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				window.close();
			}
			// Keyboard input
			if (evnt.type == Event::TextEntered) {
				if (evnt.text.unicode < 128) {
					if (evnt.text.unicode == 8) {
						if (!inputText.empty()) inputText.pop_back();
					}
					else {
						inputText.push_back(static_cast<char>(evnt.text.unicode));
					}
				}
				score += flyingText.searchAndDestroy(inputText, inputTextColor);
				if (inputTextColor == Color::Green) inputTextColor = Color(14, 119, 28);
				inputBox.setText(inputText, inputTextColor);
				scoreBox.setText("Score : " + to_string(score), Color::Red);
			}
		}

		window.clear();
		flyingText.move(dt);

		// Change difficulty if needed
		for (int i = level; i < NUM_LEVEL - 1; i++) {
			if (score > scoresBetweenLevelUp[i]) {
				level++;
				scoreLevel.setText("Level " + to_string(level + 1), gameScores[level].color);
				flyingText.accelerate(TEXT_ACCELERATION_FACTOR);
				flyingText.multiplyDisplayedWords(TEXT_MULTIPLICATION_FACTOR);
				break;
			}
		}

		scoreBox.draw(window);
		scoreLevel.draw(window);
		inputBox.draw(window);
		flyingText.draw(window);
		window.display();

		dt = clock.restart().asMilliseconds();
	}
}