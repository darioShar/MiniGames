#pragma once
#include <string>
#include <vector>


enum Color { red, blue, green, yellow, orange, white, vide};

class masterMindSet {
public:
	masterMindSet(int tries = 10);
	masterMindSet(Color a, Color b, Color c, Color d, int tries = 10);
	void putPins();
	void dispMasterMindSet(bool);
	void addToSetAndCheck(char, char, char, char);
	bool isCodeGuessed();
	bool checkValidityAndMakeCode(const std::string &answer, char&, char&, char&, char&);
	bool verify(char a);
	void showState();
	Color changeToColor(char a);
	std::string showSecretCode();
	char changeToChar(Color a);
	std::string _set = "????";
	int _tries;

	// For computer vs player
	void guess();
	void deleteImpossiblePossibilities(std::vector<std::vector<char>> &_allPossibilities);
	bool isFinished();



private:
	int _beginTries;
	std::vector<std::pair<int, int>> indications; // Stores information given to users through game.
	Color _secretCode[4];
	int _nbCorrectPinColorIncorrectPlace = 0;
	int _nbCorrectPinColorCorrectPlace = 0;
	char colors[6] = { 'R', 'G', 'B', 'Y', 'W', 'O' };

	// For computer vs player
	std::vector<std::vector<char>> _allPossibilities;
	std::vector<std::vector<char>> _pinGuesses;
	bool _isFinished;
};