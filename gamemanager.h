#pragma once

#include <iostream>
#include <vector>
#include <conio.h>
#include "board.h"
#include "boardmanager.h"

using namespace std;

class SudokuGame {
    private:
        SudokuBoard board;
        SudokuBoard solvedBoard;
        int cursorRow;
        int cursorCol;

    public:
        SudokuGame() : cursorRow(0), cursorCol(0) {}

        void initialize(SudokuBoardManager boardmanager, int numPrefilled) {
            // board = boardmanager.createNewBoard();
            board = boardmanager.importBoardFromRandomFile("Easy");
            // board.generatePreFilledNumbers(numPrefilled);
            solvedBoard = boardmanager.solveBoard(board);
            // boardmanager.exportBoardToFile(board, "template1");
        }

        void printBoard(bool isEndGame = false) const {
            for (int i = 0; i < SIZE; ++i) {
                if (i % 3 == 0 && i != 0)
                    cout << "---------------------" << endl;

                for (int j = 0; j < SIZE; ++j) {
                    if (j % 3 == 0 && j != 0)
                        cout << "| ";

                    int cellValue = board.getCellValue(i, j);

                    // Check if it's a pre-filled number and color it green
                    if (board.isPrefilled(i, j)) {
                        if (i == cursorRow && j == cursorCol)
                            cout << "\033[32m[" << cellValue << "]\033[0m "; // Green with bold for cursor
                        else
                            cout << "\033[32m" << cellValue << "\033[0m "; // Green
                    } else {
                        if (i == cursorRow && j == cursorCol && !isEndGame)
                            cout << "\033[36m[" << cellValue << "]\033[0m "; // Cyan with bold for cursor
                        else
                            cout << "\033[33m" << cellValue << "\033[0m "; // Yellow
                    }

                }
                cout << endl;
            }
        }

        bool checkWin() const {
            if (    board.isEveryRowComplete()
                    and board.isEveryColumnComplete()
                    and board.isEveryBoxComplete()  ) { 
                return true;
            }
            return false;
        }

        void moveCursor(char input) {
            if (input == 'w' && cursorRow > 0) {
                cursorRow--;
            } else if (input == 's' && cursorRow < SIZE - 1) {
                cursorRow++;
            } else if (input == 'a' && cursorCol > 0) {
                cursorCol--;
            } else if (input == 'd' && cursorCol < SIZE - 1) {
                cursorCol++;
            }
        }

        void play(const string& playerName) {
            char input;

            
            // input = _getch();

            while (true) {
                system("cls");
                cout << "Use WASD keys to move the cursor. Enter a number to fill the selected box." << endl;
                cout << "Press 'h' to autofill a square. Press 'q' to quit the game." << endl << endl;
                printBoard();
                input = _getch();
                if (input >= '1' && input <= '9' && !board.isPrefilled(cursorRow, cursorCol)) {
                    board.setCellValue(cursorRow, cursorCol, input - '0');
                } else if (input == 'h' && !board.isPrefilled(cursorRow, cursorCol)) {
                    board.setCellValue(cursorRow, cursorCol, solvedBoard.getCellValue(cursorRow, cursorCol));
                    board.markPrefilled(cursorRow, cursorCol);
                } else if (input == 'q') {
                    break;
                }

                moveCursor(input);

                if (checkWin()) {
                    printBoard(true);
                    cout << "Congratulations, " << playerName << "! You won!" << endl;
                    break;
                }
            }
        }
};