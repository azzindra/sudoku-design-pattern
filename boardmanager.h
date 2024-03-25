#pragma once

#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>
#include <dirent.h> // for directory listing
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdlib>
#include "board.h"

using namespace std;

class SudokuBoardManager {
public:
    SudokuBoard createNewBoard() {
        vector<vector<int>> initboard(SIZE, std::vector<int>(SIZE, EMPTY));
        vector<vector<bool>> initprefilledboard(SIZE, vector<bool>(SIZE, false));
        SudokuBoard newboard;
        newboard.initializeBoard(initboard, initprefilledboard);
        return newboard;
    }

    SudokuBoard solveBoard(SudokuBoard& boardToSolve) {
        solveRecursive(boardToSolve, 0, 0);
        return boardToSolve;
    }

	// New method to export the board to a text file
	void exportBoardToFile(const SudokuBoard& board, const string& fileName) {
		// Ensure the "Easy" folder exists, create it if necessary
		string folderName = "Easy";
		#ifdef _WIN32
		string command = "mkdir " + folderName;
		system(command.c_str());
		#else
		string command = "mkdir -p " + folderName;
		system(command.c_str());
		#endif

		// Open the file for writing
		ofstream file(folderName + "/" + fileName);

		if (file.is_open()) {
			// Write the board data to the file
			for (int row = 0; row < SIZE; ++row) {
				for (int col = 0; col < SIZE; ++col) {
					file << board.getCellValue(row, col) << " ";
				}
				file << endl;
			}
			file.close();
			cout << "Board exported to " << folderName << "/" << fileName << endl;
		} else {
			cerr << "Unable to open file for writing." << endl;
		}
	}

	SudokuBoard importBoardFromRandomFile(const string& difficulty) {
		string folderName = difficulty; // Easy
		vector<string> fileNames = listFilesInDirectory(folderName);
        srand(time(NULL));
		
		if (fileNames.empty()) {
			cerr << "No files found in folder " << folderName << endl;
			exit(1);
		}

		// Select a random file from the list
		string randomFileName = fileNames[rand() % fileNames.size()];
		string filePath = folderName + "/" + randomFileName;

		// Read the content of the selected file
		ifstream file(filePath);
		if (!file.is_open()) {
			cerr << "Unable to open file: " << filePath << endl;
			exit(1);
		}

		// Parse the content to reconstruct the Sudoku board
		vector<vector<int>> initBoard(SIZE, vector<int>(SIZE)); // to store temporary values
        vector<vector<bool>> initPrefilledBoard(SIZE, vector<bool>(SIZE, false)); // Assume no cells are prefilled initially
        SudokuBoard newBoard;
        newBoard.initializeBoard(initBoard, initPrefilledBoard);

		for (int row = 0; row < SIZE; ++row) {
			for (int col = 0; col < SIZE; ++col) {
				if (!(file >> initBoard[row][col])) { // fails
					cerr << "Error reading file: " << filePath << endl;
					exit(1);
				} else { // succeed
                    newBoard.setCellValue(row, col, initBoard[row][col]);
                    if (newBoard.getCellValue(row, col) != EMPTY) {
                        newBoard.markPrefilled(row, col); // Mark prefilled cells
                }
                }
            }
        }
		file.close();

		return newBoard;
	}

private:
    static bool solveRecursive(SudokuBoard& board, int row, int col) {
        if (row == SIZE) {
            row = 0;
            if (++col == SIZE)
                return true; // Entire board has been filled successfully
        }

        if (board.isPrefilled(row, col))
            return solveRecursive(board, row + 1, col);

        for (int num = 1; num <= SIZE; ++num) {
            if (board.isSafe(row, col, num)) {
                board.setCellValue(row, col, num);
                if (solveRecursive(board, row + 1, col))
                    return true;
            }
        }

        board.setCellValue(row, col, EMPTY); // Backtrack
        return false;
    }

    vector<string> listFilesInDirectory(const string& directory) {
        vector<string> fileNames;
        struct stat fileStat;
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir(directory.c_str())) != nullptr) {
            while ((ent = readdir(dir)) != nullptr) {
                string fileName = ent->d_name;
                string filePath = directory + "/" + fileName;
                if (stat(filePath.c_str(), &fileStat) == 0 && S_ISREG(fileStat.st_mode)) {
                    // Check if the file has a .txt extension
                    if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4) == ".txt") {
                        fileNames.push_back(fileName);
                    }
                }
            }
            closedir(dir);
        } else {
            cerr << "Error opening directory: " << directory << endl;
            exit(1);
        }
        return fileNames;
    }
};
