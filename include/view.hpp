#pragma once

#include "constants.hpp"
#include "errormacros.h"
#include "block.hpp"

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class View {

    public:
        View(const int & _width, const int & _length);
        ~View();

        void init();

        void clear_renderer();
        void present();

        /* draw rectangle outline */
        void display_rectangle_outline(const int & xpos, const int & ypos, const int & length, const int & width, const SDL_Color & color);

        /* draw filled rectangle */
        void display_filled_rectangle(const int & xpos, const int & ypos, const int & length, const int & width, const SDL_Color & color);

        /* write text to the window */
        void display_text_on_window(const string & str_text, const int & xpos, const int & ypos, const SDL_Color & color);

        void display_brick_on_window(const int & xpos, const int & ypos);

        void update_environment(const int & score, const vector< vector<int> > & static_positions, const Block & block);

        void display_init_screen(const Uint32 & current_time);

        void display_end_screen();


        /* intro screen animation */
        Uint32 intro_screen_prev_time;
        Uint8 intro_text_opacity;
        bool intro_opacity_increasing;


    private:
        
        SDL_Renderer *  _renderer;
        SDL_Window *    _window;
        TTF_Font *      _font;
        SDL_Texture *   _brick;

        int _width;
        int _length;

        SDL_Color get_color_from_type(const BlockType & type);

        vector<Position> get_current_position(const BlockType & _type, const int & rotation);

        void display_current_moving_block(const Block & block);

};