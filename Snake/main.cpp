#pragma warning( disable : 4244 )  
#include "Snake.h"
#include "Box.h"

#define WIDTH 1600
#define HEIGHT 900
#define SNAKE_SPEED 20.0f
#define SNAKE_SIZE 50
#define KEY_INPUT(x) Keyboard::isKeyPressed(Keyboard:: x )
#define DEFAULT_TIME 1000

struct Fruit : public RectangleShape {
	Fruit(float size = (float)SNAKE_SIZE) { setSize(Vector2f(size, size)); setFillColor(Color::Red); }
	void randomMove() { setPosition((rand() % (int)(WIDTH / getSize().x)) * getSize().x, (rand() % (int)(HEIGHT / getSize().y)) * getSize().y); }
};

int main() {
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Snake");
	window.setFramerateLimit(60);
	Event evnt;
	Clock clock;
	Snake snake(window.getSize(), SNAKE_SIZE);
	Fruit fruit((float)SNAKE_SIZE);
	Box textScore(WIDTH - (std::string("Score : ").size() + 4) * (WIDTH / 40), HEIGHT / 100,
		(std::string("Score : ").size() + 3) * (WIDTH / 40),HEIGHT / 9, WIDTH / 40); 
	textScore.setText("Score : 1");
	int score = 1;
	Box textSpeed((std::string("Speed : ").size() + 4) * (WIDTH / 40), HEIGHT / 100,
		(std::string("Speed : ").size() + 3) * (WIDTH / 40), HEIGHT / 9, WIDTH / 40);
	textSpeed.setText("Speed : 1.0");
	float speed = 1.0f;

	while (window.isOpen()) {
		while (window.pollEvent(evnt))
			if (evnt.type == Event::Closed) window.close();

		if (KEY_INPUT(Z)) snake.goUp();
		else if (KEY_INPUT(S)) snake.goDown();
		else if (KEY_INPUT(Q)) snake.goLeft();
		else if (KEY_INPUT(D)) snake.goRight();

		if (KEY_INPUT(Up)) { speed *= 1.0001f; textSpeed.setText(String("Speed : ") + String(std::to_string(speed))); }
		if (KEY_INPUT(Down)) { speed /= 1.0001f; textSpeed.setText(String("Speed : ") + String(std::to_string(speed))); }

		if (clock.getElapsedTime().asSeconds() > 1.0f / (speed * SNAKE_SPEED)) {
			if (!snake.move()) {
				textScore.setText("YOU HAVE LOST", Color::Red); 
				textScore.setPosition(((float)WIDTH - textScore.getSize().x) / 2.0f, ((float)HEIGHT - textScore.getSize().y) / 2.0f);
				textScore.draw(window); 
				window.display();
				sleep(milliseconds(1000));
				window.close(); break;
			}
			if (snake.headCollides(fruit.getGlobalBounds())) {
				fruit.randomMove();
				snake.addBody();
				textScore.setText(String("Score : ") + String(std::to_string(++score)));
			}
			window.clear();
			snake.draw(window);
			window.draw(fruit);
			textScore.draw(window);
			textSpeed.draw(window);
			window.display();
			clock.restart();
		}
		sleep(microseconds(500));
	}
}