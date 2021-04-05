#include <iostream>


#include "../include/arghelper.hpp"
#include "../include/game.hpp"

int main(int argc, char ** argv) {
    Tetris game;

    /* if help mesage wasn't printed to console, start the game */
    if (read_args(game, argc, argv)) {
        game.init_view();
        game.play();
    }
    return 0;
}