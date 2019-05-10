#include "pch.h"
#include "ClassGame.h"

#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <windows.h>



using namespace std;

ClassGame::ClassGame()
{
}


ClassGame::~ClassGame()
{
}

void ClassGame::MultiPlayer() {
	// Changes font colour to green
	system("color 0A");
	system("color 0A");

	int i = 1;
	restart = 'n';
	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;
	cout << "                   ===game rules===\n---The first player needs to type in a number bewten 1-100\n---The second player needs to guess the number\n\n" << endl;
	system("pause");

	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;
	cout << "please enter the maximum of attempts: ";
	cin >> v1;

	do {
		system("cls");
		restart1 = 'n';
		cout << "============================================================" << endl;
		cout << "           number guessing game(multiple player)" << endl;
		cout << "============================================================\n\n" << endl;
		cout << "Player 1" << endl;
		cout << "-please enter a number betwen 0-100: ";
		cin >> n1;

		if (n1 > 100) {
			cout << "\n\n***********************************************" << endl;
			cout << "ERROR\nThe number is to big please re-enter" << endl;
			cout << "\n\n***********************************************" << endl;
			system("pause");
			system("cls");
			restart1 = 'y';
		}


	} while (restart1 == 'y');

	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;
	cout << "player 2\n" << endl;
	cout << "type in the number you want to guess,type enter to start\n" << endl;
	cout << "               ======GOOD LUCK======" << endl;
	system("pause");

	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;

	cout << "               ===gamestart===\n\n" << endl;
	do {
		restart2 = 'n';
		cout << i;
		cout << ": ";
		cin >> n2;
		if (i == v1) {
			cout << "\nsorry, you are out of attempts\n\n\n" << endl;
			system("pause");
		}

		else if (n2 == n1) {
			cout << "You win!" << endl;
		}
		else if (n2 < n1) {
			cout << "bigger" << endl;
			i++;
			restart2 = 'y';
		}
		else if (n2 > n1) {
			cout << "smaller" << endl;
			i++;
			restart2 = 'y';
		}


	} while (restart2 == 'y');
	int x = 1;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                                        loading final result:\n" << endl;
	cout << "                                                                     ";
	while (x < 26) {
		cout << "=";
		Sleep(0100);
		x++;
	}
	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;

	cout << "The final result is: \n\n" << endl;
	cout << "The player 1's number is " << n1 << endl;
	cout << "number of guessing: " << i << endl;
	system("pause");

}
