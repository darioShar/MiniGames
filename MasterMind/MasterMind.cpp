// MasterMind.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include "masterMindSet.h"

using namespace std;

void ComputerGuesses();
void PlayerGuesses();

int tries = 10;

int main()
{
	//ios_base::sync_with_stdio(0);
	// Beginning counter : 
	LARGE_INTEGER counterBegin;
	QueryPerformanceCounter(&counterBegin);
	/*************************************************************/




	cout <<  setw(65) << setfill('-') << "MASTERMIND" << setw(57) << setfill('-')  << "\n\n\n";
	cout << "Beginning game...\n\n";
	string caca;
	input :
	cout << "Play against Computer (Enter A), or Computer against You (Enter B) : \n";
	cin >> caca;
	if ((caca.size() != 1) || ((toupper(caca[0]) != 'A') && (toupper(caca[0]) != 'B'))) {
		cout << "Input wrong. Please retry. \n";
		goto input;
	}
	cin.ignore();
	system("cls");
	cout << setw(65) << setfill('-') << "MASTERMIND" << setw(57) << setfill('-') << "\n\n\n";
	if (toupper(caca[0]) == 'A') {
		PlayerGuesses();
	}
	else {
		ComputerGuesses();
		//cin.ignore();
	}
	

	/*************************************************************/
	LARGE_INTEGER counterEnd;
	QueryPerformanceCounter(&counterEnd);
	cout << "\nGame ended successfully. Total duration : ";
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	cout << fixed << (double)(counterEnd.QuadPart - counterBegin.QuadPart) / (double)frequency.QuadPart;
	cout << " s.\nEnter any key to exit\n";
	// end program.
	cin.get();
    return 0;
}


void ComputerGuesses() {
	//cout << "Work in progress.\n";

	
	cout << "Choose your combination, then tell the computer how many pins of the right color are on the right place, and how many pins of right color are on the wrong place.\n";
	cout << "But first, determine the maximum number of guesses computer can do : \n";
	cin >> tries;
	if (cin.fail()) {
		cout << "\nInput wrong. Default value taken : 10.\n";
		tries = 10;
		Sleep(1000);
	}
	cin.ignore();
	tries++; // chai pas pourquoi, sinon caca.
	masterMindSet game(tries);
	for (int i = 0; i < tries, game.isFinished() == 0; i++) {
		game.guess();
	}
}


void PlayerGuesses() {
	cout << "There are 6 differents colors in this game : Red, Green, Blue, Orange, Yellow and White ! Simply enter a combination of 4 of these colors (use their initial) in order to play.\n\n";
	cout << "Computer has chosen its secret code, which will be represented by question marks.\nChoose how many tries you want (recommended 10) : ";
	cin >> tries;
	if (cin.fail()) {
		cout << "\nInput wrong. Default value taken : 10.\n";
		tries = 10;
		Sleep(1000);
	}
	cin.ignore();
	masterMindSet game(tries);  // Here, color code randomly generated, but can accept a color code as parameter. max tries can be passed as parameters.
	bool GameDone = 0;
	for (int i = 0; i < tries; i++) {
		game.showState();
		game.putPins();
		if (game.isCodeGuessed()) {
			GameDone = 1;
			i = tries; // exit loop
		}
	}
	game.showState();
	if (GameDone) {
		cout << "\n\nYou have sucessfully cracked the secret code !\n";
	}
	else {
		cout << "\n\nYou haven't been able to crack the secret code... Suck a fuck.\n";
	}
	cout << "The secret code was : " << game.showSecretCode() << "\n";


}