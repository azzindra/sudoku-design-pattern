#include <iostream>
#include "board.h"
#include "gamemanager.h"
#include "player.h"

int main() {
    SudokuGame game;
    SudokuPlayer player;
    game.initialize(player.getNumPrefilled());
    game.play(player.getName());

    return 0;
}