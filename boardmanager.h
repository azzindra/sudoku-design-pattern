#pragma once

#include <iostream>
#include <vector>
#include <time.h>
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

    SudokuBoard solveBoard(SudokuBoard boardToSolve) {
        solveRecursive(boardToSolve, 0, 0);
        return boardToSolve;
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
};
