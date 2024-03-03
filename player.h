#pragma once

#include <iostream>

using namespace std;

class SudokuPlayer {
private:
	string name;
	int numPrefilled;

public:
	SudokuPlayer() {
		cout << "Enter your name: ";
		cin >> name;
		cout << "Enter the number of prefilled numbers you want: ";
		cin >> numPrefilled;
	}

	const string& getName() const {
		return name;
	}

	int getNumPrefilled() const {
		return numPrefilled;
	}
};