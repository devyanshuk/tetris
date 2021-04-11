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
		View(const int & _width, const int & _length, bool _show_hint);
		~View();

		/* forbid copy constructor */
		View& operator = (View const &) = delete;

		void init_animation_fields();

		void clear_renderer();
		void present_renderer();

		/* draw rectangle outline */
		void display_rectangle_outline(Position pos, int length, int width, SDL_Color color);

		/* draw filled rectangle */
		void display_filled_rectangle(Position pos, int length, int width, SDL_Color color);

		/* write text to the window */
		void display_text_on_window(string str_text, Position pos, SDL_Color color);

		void display_brick_on_window(Position pos);

		void update_environment(int score, const vector< vector<int> > & static_positions, const Block & block);

		void display_init_screen(Uint32 current_time);

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
		bool _show_hint;
		
		void display_block(Position pos, SDL_Color color);

		SDL_Color get_color_from_type(const BlockType & type);

		vector<Position> get_current_position(const BlockType & _type, const int & rotation);

		void display_current_moving_block(const Block & block);

		void display_moving_block_final_pos(const Block & test_block, const std::vector< std::vector<int> > & static_positions);

		void display_blocks_and_board(const std::vector< std::vector<int> > & static_positions);

};
