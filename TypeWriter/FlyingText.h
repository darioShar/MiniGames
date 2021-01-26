#pragma once
#include <SFML\Graphics.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "filepaths.h"


#define FLYING_CHARACTER_SIZE 30
#define DEFAULT_SPACE 100.0f

#define NUM_COLORS 5
#define COLOR_VALUE_WHITE 10
#define COLOR_PROBABILITY_WHITE 0.5f
#define COLOR_VALUE_GREEN 15
#define COLOR_PROBABILITY_GREEN 0.2f
#define COLOR_VALUE_BLUE 25
#define COLOR_PROBABILITY_BLUE 0.15f
#define COLOR_VALUE_PURPLE 50
#define COLOR_PROBABILITY_PURPLE 0.10f
#define COLOR_VALUE_ORANGE 100
#define COLOR_PROBABILITY_ORANGE 0.05f


using namespace sf;
using namespace std;

struct GameScore {
	GameScore(Color col, int val, float prob) : color(col), value(val), probability(prob) {}
	Color color;
	int value;
	float probability;
};

extern GameScore gameScores[NUM_COLORS];

class FlyingText
{
	// Screen size
	int m_width;
	int m_height;

	// Speed...
	float m_speed;

	// dictionary
	vector<string> m_dictionary;

	// Now for graphic display
	Font m_font;
	vector<Text> m_displayedWords;

	Color generateColor();
	void addWord();

public:
	FlyingText(int, int, float, int);
	~FlyingText();

	void loadDictionary(string const &);
	void loadWords(int);
	void multiplyDisplayedWords(float factor);
	int searchAndDestroy(string &, Color &);

	void accelerate(float factor);
	void move(float dt);

	void draw(RenderWindow &);
};

