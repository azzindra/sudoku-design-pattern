#pragma once

#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

const int SIZE = 9;
const int EMPTY = 0;

class SudokuBoard {
    private:
        vector<vector<int>> board;
        vector<vector<bool>> prefilled;

    public:
        SudokuBoard() : board(SIZE, vector<int>(SIZE, EMPTY)), prefilled(SIZE, vector<bool>(SIZE, false)) {}

        void setCellValue(int row, int col, int value) {
            board[row][col] = value;
        }

        int getCellValue(int row, int col) const {
            return board[row][col];
        }

        void markPrefilled(int row, int col) {
            prefilled[row][col] = true;
        }

        bool isPrefilled(int row, int col) const {
            return prefilled[row][col];
        }

        void generatePreFilledNumbers(int numPrefilled) {
            srand(time(nullptr));
            int count = 0;
            while (count < numPrefilled) {
                int row = rand() % SIZE;
                int col = rand() % SIZE;
                int num = rand() % SIZE + 1;
                if (board[row][col] == EMPTY && isSafe(row, col, num)) {
                    setCellValue(row, col, num);
                    markPrefilled(row, col);
                    ++count;
                }
            }
        }

        bool isSafe(int row, int col, int num) const {
            return !isNumberInRow(row, num) &&
                !isNumberInColumn(col, num) &&
                !isNumberInBox(row - row % 3, col - col % 3, num);
        }

        bool isNumberInRow(int row, int num) const {
            for (int col = 0; col < SIZE; ++col) {
                if (board[row][col] == num)
                    return true;
            }
            return false;
        }

        bool isNumberInColumn(int col, int num) const {
            for (int row = 0; row < SIZE; ++row) {
                if (board[row][col] == num)
                    return true;
            }
            return false;
        }

        bool isNumberInBox(int startRow, int startCol, int num) const {
            for (int row = 0; row < 3; ++row) {
                for (int col = 0; col < 3; ++col) {
                    if (board[row + startRow][col + startCol] == num)
                        return true;
                }
            }
            return false;
        }
};