#include <iostream>
#include "board.h"
#include "gamemanager.h"
#include "player.h"
#include "boardmanager.h"

int main() {
    SudokuGame game;
    SudokuPlayer player;
    SudokuBoardManager boardmanager;
    game.initialize(boardmanager, player.getNumPrefilled());
    game.play(player.getName());

    return 0;
}