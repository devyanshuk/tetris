#pragma once

#include "view.hpp"
#include "block.hpp"
#include "constants.hpp"
#include "arghelper.hpp"
#include "errormacros.h"

#include <SDL2/SDL.h>
#include <memory>
#include <time.h>
#include <optional>
#include <algorithm>
#include <stdlib.h>
#include <vector>

enum GameStates
{
	GAMESTATE_INIT_SCREEN = 0,
	GAMESTATE_PLAYING,
	GAMESTATE_END
};

class Tetris {

	public:

		Tetris(ArgInput input);

		/* forbid copy constructor */
		Tetris& operator = (Tetris const &) = delete;
		Tetris& operator =  (Tetris &&) = delete;
		Tetris(Tetris const &) = delete;
		Tetris(Tetris &&) = delete;

		bool        init();
		int         play();

		/* sets all values of _non_moving_blocks to -1 indicating free space */
		void        init_non_moving_blocks();
		void        init_view();

	private:
		/* values 0..7 in _non_moving_blocks represent static blocks of type 0..7 . -1 represents free space */
		std::vector< std::vector<std::optional<int> > > _non_moving_blocks;
		std::unique_ptr<View>           _view;
		std::optional<SDL_Keycode>      _key;

		int         _score;
		GameStates  _state;
		Block       _current_active_block;
		Uint32      _block_vertical_update_speed;
		bool        make_new_block();
		bool        try_moving_piece(Block test_block);
		bool        check_collision(int new_x_pos, int new_y_pos, int new_rotation);
		Uint32      _prev_y_update_time;
		Uint32      _prev_x_update_time;
		bool        update_screen();
		void        remove_all_complete_rows();

		/* */
		bool        is_collision(const Block & test_block);
		Uint32      get_tick_difference();
		bool        update_game();
		void        update_game_state();

};
