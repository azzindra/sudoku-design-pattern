#include <iostream>
#include <vector>
#include <conio.h>
#include <time.h>

using namespace std;

const int SIZE = 9;
const int EMPTY = 0;

void printBoard(const vector<vector<int>>& board, const vector<vector<bool>>& prefilled,
                int cursorRow, int cursorCol, const vector<vector<int>>& solvedBoard,
                bool isEndGame = false) {
    for (int i = 0; i < SIZE; ++i) {
        if (i % 3 == 0 && i != 0)
            cout << "---------------------" << endl;

        for (int j = 0; j < SIZE; ++j) {
            if (j % 3 == 0 && j != 0)
                cout << "| ";

            // Check if it's a pre-filled number and color it green
            if (prefilled[i][j]) {
                if (i == cursorRow && j == cursorCol)
                    cout << "\033[32m[" << board[i][j] << "]\033[0m "; // Green with bold for cursor
                else
                    cout << "\033[32m" << board[i][j] << "\033[0m "; // Green
            } else {
                if (i == cursorRow && j == cursorCol && !isEndGame)
                    cout << "\033[36m[" << board[i][j] << "]\033[0m "; // Cyan with bold for cursor
                else
                    cout << "\033[33m" << board[i][j] << "\033[0m "; // Yellow
            }
        }
        cout << endl;
    }
}

// Function to check if a number is already present in a row
bool isNumberInRow(const vector<vector<int>>& board, int row, int num) {
    for (int col = 0; col < SIZE; ++col) {
        if (board[row][col] == num)
            return true;
    }
    return false;
}

// Function to check if a number is already present in a column
bool isNumberInColumn(const vector<vector<int>>& board, int col, int num) {
    for (int row = 0; row < SIZE; ++row) {
        if (board[row][col] == num)
            return true;
    }
    return false;
}

// Function to check if a number is already present in a 3x3 grid
bool isNumberInBox(const vector<vector<int>>& board, int startRow, int startCol, int num) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row + startRow][col + startCol] == num)
                return true;
        }
    }
    return false;
}

// Function to check if it's safe to place a number in a given cell
bool isSafe(const vector<vector<int>>& board, int row, int col, int num) {
    return !isNumberInRow(board, row, num) &&
           !isNumberInColumn(board, col, num) &&
           !isNumberInBox(board, row - row % 3, col - col % 3, num);
}

// Function to solve the Sudoku using backtracking
bool solveSudoku(vector<vector<int>>& board) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            if (board[row][col] == EMPTY) {
                for (int num = 1; num <= SIZE; ++num) {
                    if (isSafe(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku(board))
                            return true;
                        board[row][col] = EMPTY; // Backtrack
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// Function to randomly generate pre-filled numbers on the Sudoku board
void generatePreFilledNumbers(vector<vector<int>>& board, int numPrefilled) {
    srand(time(nullptr));
    int count = 0;
    while (count < numPrefilled) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        int num = rand() % SIZE + 1;
        if (board[row][col] == EMPTY && isSafe(board, row, col, num)) {
            board[row][col] = num;
            ++count;
        }
    }
}

bool checkWin(const vector<vector<int>>& board) {
    // Check each row
    for (int row = 0; row < SIZE; ++row) {
        for (int num = 1; num <= SIZE; ++num) {
            if (!isNumberInRow(board, row, num))
                return false; // Missing number in this row
        }
    }

    // Check each column
    for (int col = 0; col < SIZE; ++col) {
        for (int num = 1; num <= SIZE; ++num) {
            if (!isNumberInColumn(board, col, num))
                return false; // Missing number in this column
        }
    }

    // Check each 3x3 box
    for (int startRow = 0; startRow < SIZE; startRow += 3) {
        for (int startCol = 0; startCol < SIZE; startCol += 3) {
            for (int num = 1; num <= SIZE; ++num) {
                if (!isNumberInBox(board, startRow, startCol, num))
                    return false; // Missing number in this box
            }
        }
    }

    return true; // Board is valid according to Sudoku rules
}

int main() {
    vector<vector<int>> board(SIZE, vector<int>(SIZE, EMPTY));
    vector<vector<bool>> prefilled(SIZE, vector<bool>(SIZE, false));
    vector<vector<int>> solvedBoard(SIZE, vector<int>(SIZE, EMPTY)); // Store solved board

    // Generate pre-filled numbers
    generatePreFilledNumbers(board, 20);

    // Solve Sudoku to get the solution
    solvedBoard = board; // Copy original board
    solveSudoku(solvedBoard);

    // Mark pre-filled cells
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] != EMPTY) {
                prefilled[i][j] = true;
            }
        }
    }

    int cursorRow = 0, cursorCol = 0;
    char input;

    cout << "Use WASD keys to move the cursor. Enter a number to fill the selected box. Press 'h' to autofill a square." << endl;

    while (true) {
        system("cls");
        printBoard(board, prefilled, cursorRow, cursorCol, solvedBoard);
        input = _getch();
        if (input == 'w' && cursorRow > 0) {
            cursorRow--;
        } else if (input == 's' && cursorRow < SIZE - 1) {
            cursorRow++;
        } else if (input == 'a' && cursorCol > 0) {
            cursorCol--;
        } else if (input == 'd' && cursorCol < SIZE - 1) {
            cursorCol++;
        } else if (input >= '1' && input <= '9' && !prefilled[cursorRow][cursorCol]) {
            board[cursorRow][cursorCol] = input - '0';
        } else if (input == 'h' && !prefilled[cursorRow][cursorCol]) {
            // Autofill the square with the correct number from solved board
            board[cursorRow][cursorCol] = solvedBoard[cursorRow][cursorCol];
            prefilled[cursorRow][cursorCol] = true; // Mark as autofilled
        } else if (input == 'q') {
            break;
        }

        // Check if the player has won
        if (checkWin(board)) {
            system("cls");
            printBoard(board, prefilled, cursorRow, cursorCol, solvedBoard, true);
            cout << "Congratulations! You won!" << endl;
            break;
        }
    }

    return 0;
}
