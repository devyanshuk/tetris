#pragma once

#include "view.hpp"
#include "block.hpp"
#include "constants.hpp"
#include "arghelper.hpp"
#include "errormacros.h"

#include <SDL2/SDL.h>
#include <time.h>
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
        ~Tetris();
        int         init();
        int        play();
        void        update_game(const Uint32 & delta_time);

        /* sets all values of _static_blocks to -1 indicating free space */
        void        init_static_blocks();
        void        init_view();

    private:

        int         _score;
        View *      _view;
        GameStates  _state;
        Block       _current_active_block;
        Uint32      _block_update_speed;
        bool        make_new_block();
        Uint32      _prev_time;
        void        update_screen(const SDL_Keycode & key, const Uint32 & delta_time);

        /* */
        bool        is_intersecting();

        /* values 0..7 in _static_blocks represent static blocks of type 0..7 . -1 represents free space */
        std::vector< std::vector<int> > _static_blocks;
        Uint32 get_tick_difference();

};