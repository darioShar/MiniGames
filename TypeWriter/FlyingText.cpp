#include "FlyingText.h"


GameScore gameScores[NUM_COLORS] = {
	GameScore(Color::White, COLOR_VALUE_WHITE, COLOR_PROBABILITY_WHITE),
	GameScore(Color(2, 219, 5), COLOR_VALUE_GREEN, COLOR_PROBABILITY_GREEN),
	GameScore(Color::Blue, COLOR_VALUE_BLUE, COLOR_PROBABILITY_BLUE),
	GameScore(Color::Green, COLOR_VALUE_PURPLE, COLOR_PROBABILITY_PURPLE),
	GameScore(Color(255, 205, 5), COLOR_VALUE_ORANGE, COLOR_PROBABILITY_ORANGE)
};




FlyingText::FlyingText(int width, int height, float speed, int dispWords)
	:	m_width(width)
	,	m_height(height)
	,	m_speed(speed)
{
	m_font.loadFromFile(FONT_PATH);
	loadDictionary(DICTIONARY_PATH);
	loadWords(dispWords);
}


FlyingText::~FlyingText()
{
}

void FlyingText::loadDictionary(string const & filePath) {
	// open file
	ifstream file(filePath);
	if (file) {
		string line;
		m_dictionary.clear();
		while (getline(file, line)) {
			m_dictionary.push_back(line);
		}
	}
}

void FlyingText::loadWords(int nbWords) {
	nbWords = min(static_cast<int>(m_dictionary.size()), nbWords);
	m_displayedWords.clear();
	for (int i = 0; i < nbWords; i++) addWord();
}

void FlyingText::multiplyDisplayedWords(float factor) {
	int dispWords = m_displayedWords.size() * factor;
	loadWords(dispWords);
}


int FlyingText::searchAndDestroy(string &str, Color &col) {
	// if it is
	auto it = find_if(m_displayedWords.begin(), m_displayedWords.end(), [&](Text &a) { return a.getString() == str; });
	if (it != m_displayedWords.end()) {
		int score = (*it).getString().getSize();
		for (auto& x : gameScores) if ((*it).getFillColor() == x.color) { score *= x.value; break; }
		m_displayedWords.erase(it);
		addWord();
		str.clear();
		col = Color::Black;
		return score;
	}
	// if contains
	it = find_if(m_displayedWords.begin(), m_displayedWords.end(), [&](Text &a) { return a.getString().find(str) != string::npos; });
	if (it != m_displayedWords.end()) {
		col = (*it).getFillColor();
		return 0;
	}
	col = Color::Red;
	return 0;
}

void FlyingText::addWord() {
	if (m_displayedWords.size() < m_dictionary.size()) {
		string newWord;
		do {
			newWord = m_dictionary[rand() % m_dictionary.size()];
		} while (find_if(m_displayedWords.begin(), m_displayedWords.end(), [&](Text &a) { return a.getString() == newWord; } ) != m_displayedWords.end());
		Text t(newWord, m_font, FLYING_CHARACTER_SIZE);
		t.setPosition(Vector2f((-DEFAULT_SPACE * (rand() % 10)) - DEFAULT_SPACE, rand() % (m_height - FLYING_CHARACTER_SIZE - 10) + 5));
		t.setFillColor(generateColor());
		m_displayedWords.push_back(t);
	}
}

void FlyingText::accelerate(float factor) {
	m_speed *= factor;
}


void FlyingText::move(float dt) {
	for (auto& dw : m_displayedWords) {
		dw.move(sf::Vector2f(m_speed * dt, 0.0f));
		Vector2f pos = dw.getPosition();
		if (pos.x > m_width) {
			pos.x = (-50.0f * (rand() % 5)) - 50.0f;
			dw.setPosition(pos);
		}
	}
}

void FlyingText::draw(RenderWindow &window) {
	for (auto& x : m_displayedWords) window.draw(x);
}



Color FlyingText::generateColor() {
	float prob = (float)rand() / (float)RAND_MAX;
	float sumProb = 0.0f;
	for (int i = 0; i < NUM_COLORS; i++) {
		sumProb += gameScores[i].probability;
		if (sumProb > prob) return gameScores[i].color;
	}
}
