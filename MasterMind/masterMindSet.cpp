#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <vector>
#include "masterMindSet.h"


masterMindSet::masterMindSet(int tries)
{
	srand(time(0));
	for (int i = 0; i < 4; i++) {
		_secretCode[i] = changeToColor(colors[rand() % 6]);
	}
	_tries = tries;
	_beginTries = tries;
	std::vector<char> remplir(4);
	for (int i = 0; i < 1296; i++) {
		remplir[0] = colors[i % 6];
		remplir[1] = colors[(i % 36) / 6];
		remplir[2] = colors[(i % 216) / 36];
		remplir[3] = colors[(i % 1296) / 216];
		_allPossibilities.push_back(remplir);
	}
	_isFinished = 0;
}


masterMindSet::masterMindSet(Color a, Color b, Color c, Color d, int tries)
{
	_secretCode[0] = a;
	_secretCode[1] = b;
	_secretCode[2] = c;
	_secretCode[3] = d;
	_tries = tries;
	_beginTries = tries;
	std::vector<char> remplir(4);
	for (int i = 0; i < 1296; i++) {
		remplir[0] = colors[i % 6];
		remplir[1] = colors[(i % 36) / 6];
		remplir[2] = colors[(i % 216) / 36];
		remplir[3] = colors[(i % 1296) / 216];
		_allPossibilities.push_back(remplir);
	}
	_isFinished = 0;
}


void masterMindSet::dispMasterMindSet(bool questionMarks)
{
	if (questionMarks) {
		std::cout << "????\n";
		for (int i = 4; i < _set.size(); i += 4) {
			for (int j = 0; j < 4; j++) {
				printf("%c ", _set[i + j]);
			}
			if ((indications.size() > 0) && (_set.size()> 4)) {
				std::cout << " (" << indications[i / 4 - 1].first << " right color and place, " << indications[i / 4 - 1].second << " right color wrong place)";
			}
			printf("\n");
		}
	}
	else {
		for (int i = 4; i < _set.size(); i += 4) {
			for (int j = 0; j < 4; j++) {
				printf("%c ", _set[i + j]);
			}
			if ((indications.size() > 0) && (_set.size() - i > 4)) {
				std::cout << " (" << indications[i / 4 - 1].first << " right color and place, " << indications[i / 4 - 1].second << " right color wrong place)";
			}
			printf("\n");
		}
	}
}


void masterMindSet::showState()
{
	system("cls");
	std::cout << std::setw(65) << std::setfill('-') << "MASTERMIND" << std::setw(57) << std::setfill('-') << "\n\n\n";
	std::cout << "6 colors available : (R)ed, (G)reen, (B)lue, (Y)ellow, (W)hite, (O)range.\n\n";
	std::cout << _tries << " trie(s) left to guess the code.\n\n";
	dispMasterMindSet(1);
}


std::string masterMindSet::showSecretCode()
{
	std::string code("");
	for (int i = 0; i < 4; i++) {
		code += changeToChar(_secretCode[i]);
		code += ' ';
	}
	return code;
}


void masterMindSet::putPins()
{
	//std::cout << "Enter colors : \n";
	//dispMasterMindSet();
	char a = 'a', b = 'a', c = 'a', d = 'a';
	std::string answer;
	//std::cin.ignore();
	std::getline(std::cin, answer);
	if (checkValidityAndMakeCode(answer, a, b, c, d) == 0) {
		std::cout << "\nColor code entered ambiguous or incorrect. Please retry.\n\n";
		putPins();
		return;
	}
	a = toupper(a);
	b = toupper(b);
	c = toupper(c);
	d = toupper(d);
	int verification = 0;
	verification += verify(a);
	verification += verify(b);
	verification += verify(c);
	verification += verify(d);
	if (verification == 0) {
		addToSetAndCheck(a, b, c, d); 
		//std::cout << "Total number of proposition : " << _tries << ".\n\n\n";
		_tries--;
		return;
	}
	else {
		std::cout << "\nColor code entered ambiguous or incorrect. Please retry.\n\n";
		putPins();
		return;
	}
}


void masterMindSet::addToSetAndCheck(char aPute, char bPute, char cPute, char dPute)
{
	_set += aPute;
	_set += bPute;
	_set += cPute;
	_set += dPute;
	Color a = changeToColor(aPute);
	Color b = changeToColor(bPute);
	Color c = changeToColor(cPute);
	Color d = changeToColor(dPute);
	_nbCorrectPinColorCorrectPlace = 0;
	_nbCorrectPinColorIncorrectPlace = 0;
	Color codeEntered[4] = { a,b,c,d };
	Color secretCodeCopy[4];
	for (int i = 0; i < 4; i++) {
		secretCodeCopy[i] = _secretCode[i];
	}
	
	for (int i = 0; i < 4; i++) {
		if (codeEntered[i] == secretCodeCopy[i]) {
			_nbCorrectPinColorCorrectPlace++;
			secretCodeCopy[i] = vide;
			codeEntered[i] = vide;
		}
	}
	int i = 0;
	beginCheck :
	for (; i < 4; i++) {
		if (secretCodeCopy[i] != vide) {
			for (int j = 0; j < 4; j++) {
				if (codeEntered[j] != vide) {
					if (codeEntered[j] == secretCodeCopy[i]) {
						codeEntered[j] = vide;
						secretCodeCopy[i] = vide;
						_nbCorrectPinColorIncorrectPlace++;
						i++;
						goto beginCheck;
					}
				}
			}
		}
	}

	indications.push_back(std::make_pair(_nbCorrectPinColorCorrectPlace, _nbCorrectPinColorIncorrectPlace));
}





bool masterMindSet::isCodeGuessed()
{
	if (_nbCorrectPinColorCorrectPlace == 4) {
		return true;
	}
	else {
		return false;
	}
}


bool masterMindSet::checkValidityAndMakeCode(const std::string &answer, char& a, char& b, char& c, char& d) {
	int i = 0;
	for (; i < answer.size(); i++) {
		if (isalpha(answer[i])) {
			a = answer[i];
			i++;
			break;
		}
	}
	for (; i < answer.size(); i++) {
		if (isalpha(answer[i])) {
			b = answer[i];
			i++;
			break;
		}
	}
	for (; i < answer.size(); i++) {
		if (isalpha(answer[i])) {
			c = answer[i];
			i++;
			break;
		}
	}
	for (; i < answer.size(); i++) {
		if (isalpha(answer[i])) {
			d = answer[i];
			i++;
			break;
		}
	}
	for (; i < answer.size(); i++) {
		if (isalpha(answer[i])) {
			return 0;
		}
	}
	return 1; // all good.
}

bool masterMindSet::verify(char a) {
	if (a == 'R' || a == 'G' || a == 'B' || a == 'W' || a == 'Y' || a == 'O') {
		return 0;
	}
	else {
		return 1;
	}
}

Color masterMindSet::changeToColor(char a) {
	Color couleur;
	switch (a) {
	case 'R':
		couleur = red;
		return couleur;
		break;
	case 'G':
		couleur = green;
		return couleur;
		break;
	case 'B':
		couleur = blue;
		return couleur;
		break;
	case 'Y':
		couleur = yellow;
		return couleur;
		break;
	case 'W':
		couleur = white;
		return couleur;
		break;
	case 'O':
		couleur = orange;
		return couleur;
		break;
	}
}


char masterMindSet::changeToChar(Color a)
{
	switch (a) {
	case red:
		return 'R';
		break;
	case green:
		return 'G';
		break;
	case blue:
		return 'B';
		break;
	case yellow:
		return 'Y';
		break;
	case white:
		return 'W';
		break;
	case orange:
		return 'O';
		break;
	case vide :  // la ce serait caca.
		std::cerr << "ERROR : Tried to change Color 'vide' to char.";
		return '\0';
		break;
	}
}

void masterMindSet::guess()
{
	/*
	Algorithm for finding guess.
	will use indication[], pinGuesses[]...
	One can simply create a list of all different answers, try a guess, see the result, and erase all elements from 
	the list that would not have produced same result (i.e same user input.).
	*/
	// Here it will be creation of the list etc.
	if (_pinGuesses.size() == 0) {
		//_pinGuesses.push_back(_allPossibilities[rand() % 1296]);
		// On va plutot commencer avec une autre combinaison :
		_pinGuesses.push_back({ 'R', 'R', 'B', 'B' });
	}
	else {
		deleteImpossiblePossibilities(_allPossibilities);
		if (_allPossibilities.size() >= 1) {
			_pinGuesses.push_back(_allPossibilities[0]);
		}
		else {
			//std::cerr << "ERROR_IN_ALGORITHM !";
			_isFinished = 1;
			std::cout << "USER HAS PROVIDED FAKE INFORMATION : NO CODE CAN MATCH PREVIOUS INDICATIONS.";
			return;
		}
	}


	_set += _pinGuesses[_beginTries - _tries][0];
	_set += _pinGuesses[_beginTries - _tries][1];
	_set += _pinGuesses[_beginTries - _tries][2];
	_set += _pinGuesses[_beginTries - _tries][3];

	if (_allPossibilities.size() == 1) {
		_tries = 1; // Because the next if will decrement it.
		_isFinished = 1;
	}


	if (--_tries > 0) {
		system("cls");
		std::cout << std::setw(65) << std::setfill('-') << "MASTERMIND" << std::setw(57) << std::setfill('-') << "\n\n\n";
		std::cout << "New state of the gameset : \n";
		dispMasterMindSet(0);

	input1:
		std::cout << "\nIn the last line, how many pins are of the right color and on the right place ?\n";
		std::cin >> _nbCorrectPinColorCorrectPlace;
		if (std::cin.fail() || _nbCorrectPinColorCorrectPlace > 4 || _nbCorrectPinColorCorrectPlace < 0) {
			std::cout << "Input wrong. Please retry. \n";
			goto input1;
		}
		std::cin.ignore();

	input2:
		std::cout << "\nIn the last line, how many pins are of the right color and on the wrong place ?\n";
		std::cin >> _nbCorrectPinColorIncorrectPlace;
		if (std::cin.fail() || _nbCorrectPinColorIncorrectPlace > 4 || _nbCorrectPinColorIncorrectPlace < 0) {
			std::cout << "Input wrong. Please retry. \n";
			goto input2;
		}
		std::cin.ignore();


		indications.push_back(std::make_pair(_nbCorrectPinColorCorrectPlace, _nbCorrectPinColorIncorrectPlace));
	}
	else if (_tries == 0) {
		system("cls");
		std::cout << std::setw(65) << std::setfill('-') << "MASTERMIND" << std::setw(57) << std::setfill('-') << "\n\n\n";
		std::cout << "Final state of the gameset : \n";
		dispMasterMindSet(0);
		std::cout << "Was your code " << _set[_set.size() - 4] << _set[_set.size() - 3] << _set[_set.size() - 2] << _set[_set.size() - 1] << " ?";
		Sleep(1000);
		std::cout << "\nWell, of course it is. And you can go suck a fuck.\n\n";
	}
	return;
}

void masterMindSet::deleteImpossiblePossibilities(std::vector<std::vector<char>>& _allPossibilities)
{
	std::vector<char> lastCode = _pinGuesses[_pinGuesses.size() - 1];
	std::vector<std::vector<char>> possibTries = _allPossibilities;

	std::vector<int> toDelete;
	for (int i = 0; i < possibTries.size(); i++) {
		lastCode = _pinGuesses[_pinGuesses.size() - 1];
		_nbCorrectPinColorCorrectPlace = 0;
		_nbCorrectPinColorIncorrectPlace = 0;
		for (int j = 0; j < 4; j++) {
			if (lastCode[j] == possibTries[i][j]) {
				_nbCorrectPinColorCorrectPlace++;
				possibTries[i][j] = '\0';
				lastCode[j] = '\0';
			}
		}
		int j = 0;
	begCheck2:
		for (; j < 4; j++) {
			if (possibTries[i][j] != '\0') {
				for (int k = 0; k < 4; k++) {
					if (lastCode[k] != '\0') {
						if (lastCode[k] == possibTries[i][j]) {
							lastCode[k] = '\0';
							possibTries[i][j] = '\0';
							_nbCorrectPinColorIncorrectPlace++;
							j++;
							goto begCheck2;
						}
					}
				}
			}
		}
		if (std::make_pair(_nbCorrectPinColorCorrectPlace, _nbCorrectPinColorIncorrectPlace) != indications[indications.size() - 1]) {
			//std::cout << "Erasing " << &_allPossibilities[i];
			//_allPossibilities.erase(_allPossibilities.begin() + i);
			toDelete.push_back(i);
		}
	}
	for (int i = 0; i < toDelete.size(); i++) {
		_allPossibilities.erase(_allPossibilities.begin() + toDelete[i] - i);   // - i because at each iteration _allPossibilities size decrement.
	}

}

bool masterMindSet::isFinished()
{
	return _isFinished;
}
