#include "../include/arghelper.hpp"
#include "../include/game.hpp"

int main(int argc, char ** argv) {

	ArgInput input;

	/* if help mesage wasn't printed to console, start the game */
	if (!read_args(input, argc, argv)) {
		return 0;
	}

	Tetris game(input);
	return game.play();
}
