#pragma once

#include <iostream>
#include <vector>
#include <conio.h>
#include "board.h"

using namespace std;

class SudokuSolver {
    public:
        static bool solve(SudokuBoard& board);
    private:
        static bool solveRecursive(SudokuBoard& board, int row, int col);
    };

    bool SudokuSolver::solve(SudokuBoard& board) {
        return solveRecursive(board, 0, 0);
}

bool SudokuSolver::solveRecursive(SudokuBoard& board, int row, int col) {
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

class SudokuGame {
    private:
        SudokuBoard board;
        SudokuBoard solvedBoard;
        int cursorRow;
        int cursorCol;

    public:
        SudokuGame() : cursorRow(0), cursorCol(0) {}

        void initialize(int numPrefilled) {
            board.generatePreFilledNumbers(numPrefilled);
            solvedBoard = board;
            SudokuSolver::solve(solvedBoard);
        }

        void print(bool isEndGame = false) const {
            system("cls");
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
            for (int row = 0; row < SIZE; ++row) {
                for (int num = 1; num <= SIZE; ++num) {
                    if (!board.isNumberInRow(row, num))
                        return false; // Missing number in this row
                }
            }

            for (int col = 0; col < SIZE; ++col) {
                for (int num = 1; num <= SIZE; ++num) {
                    if (!board.isNumberInColumn(col, num))
                        return false; // Missing number in this column
                }
            }

            for (int startRow = 0; startRow < SIZE; startRow += 3) {
                for (int startCol = 0; startCol < SIZE; startCol += 3) {
                    for (int num = 1; num <= SIZE; ++num) {
                        if (!board.isNumberInBox(startRow, startCol, num))
                            return false; // Missing number in this box
                    }
                }
            }

            return true; // Board is valid according to Sudoku rules
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

            cout << "Use WASD keys to move the cursor. Enter a number to fill the selected box. Press 'h' to autofill a square." << endl;

            while (true) {
                print();
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
                    print(true);
                    cout << "Congratulations, " << playerName << "! You won!" << endl;
                    break;
                }
            }
        }
};