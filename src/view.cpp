#include "../include/view.hpp"
#include <iostream>

View::View(const int & _width, const int & _length, bool _show_hint):
intro_screen_prev_time(0),
intro_text_opacity(255),
intro_opacity_increasing(false)
{
	/* initialize sdl */
	CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

	/* initialize window and renderer */
	SDL_CreateWindowAndRenderer(_width, _length, SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC, &_window, &_renderer);
	CHECK_ERROR(_window == NULL, SDL_GetError());
	CHECK_ERROR(_renderer == NULL, SDL_GetError());

	SDL_SetWindowTitle(_window, WINDOW_TITLE);

	/* initialize font */
	CHECK_ERROR(TTF_Init() == -1, TTF_GetError());
	_font = TTF_OpenFont(FONT_LOCATION, FONT_SIZE);
	CHECK_ERROR(_font == NULL, TTF_GetError());

	/* initialize image texture */
	CHECK_ERROR(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0, IMG_GetError());

	SDL_Surface * image = IMG_Load(WALL_IMAGE_LOCATION);
	CHECK_ERROR(image == NULL, IMG_GetError());

	_brick = SDL_CreateTextureFromSurface(_renderer, image);
	CHECK_ERROR(_brick == NULL, IMG_GetError());
	SDL_FreeSurface(image);

	this->_width = _width;
	this->_length = _length;
	this->_show_hint = _show_hint;
}

View::~View()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_DestroyTexture(_brick);
	TTF_CloseFont(_font);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void View::init_animation_fields() {
	/* initialize intro screen animation helpers */
	intro_screen_prev_time = SDL_GetTicks();
	intro_text_opacity = 255;
	intro_opacity_increasing = false;
}

void View::display_brick_on_window(Position pos) {
	SDL_Rect destination = { pos.x, pos.y, BLOCK_WIDTH, BLOCK_HEIGHT };
	SDL_RenderCopy(_renderer, _brick, NULL, &destination);
}

void View::display_init_screen(Uint32 current_time) {

	/* background */
	display_filled_rectangle( ORIGIN, _length, _width, BACKGROUND_COLOR );


	if (current_time > intro_screen_prev_time + 2) {
		intro_screen_prev_time = current_time;
		intro_text_opacity += intro_opacity_increasing ? 1 : -1;
		intro_opacity_increasing = (intro_text_opacity >= 255) ? false :
		(intro_text_opacity <= 50) ? true :
		intro_opacity_increasing;
	}

	SDL_Color init_screen_color = { .r = INIT_SCREEN_COLOR.r, .g = INIT_SCREEN_COLOR.g, .b = INIT_SCREEN_COLOR.b, .a = intro_text_opacity };

	/* init screen text */
	display_text_on_window("Press spacebar to continue", INIT_TEXT_POS, init_screen_color);
}

void View::display_moving_block_final_pos(const Block & test_block, const std::vector< std::vector<int> > & static_positions) {
	Block block = test_block;
	std::vector<Position> all_pos = Block::get_current_position(block._block_type, block._rotation);
	SDL_Color color = Block::get_color_from_type(block._block_type);
	color.a = 170;

	while (!Block::is_collision(block, static_positions)) {
		block._pos.y++;
	}
	block._pos.y--;
	
	for (const Position & pos : all_pos) {
		int curr_x = BOARD_OFFSET_X + BLOCK_WIDTH * (block._pos.x + pos.x);
		int curr_y = BOARD_OFFSET_Y + BLOCK_HEIGHT * (block._pos.y + pos.y);
		Position curr_pos = { curr_x, curr_y };

		display_filled_rectangle( curr_pos, BLOCK_WIDTH, BLOCK_HEIGHT , color );
	}

}

void View::display_block(Position pos, SDL_Color color) {
	Position inner_square = { .x = pos.x + 5, .y = pos.y + 5 };
	display_filled_rectangle( pos, BLOCK_WIDTH, BLOCK_HEIGHT, BLOCK_OUTLINE_COLOR );
	display_filled_rectangle( inner_square, BLOCK_WIDTH - 10, BLOCK_HEIGHT - 10, color );
}

void View::display_current_moving_block(const Block & block) {
	std::vector<Position> display_pos = Block::get_current_position(block._block_type, block._rotation);
	SDL_Color color = Block::get_color_from_type(block._block_type);

	for (const Position & pos : display_pos) {
		int curr_x = BOARD_OFFSET_X + BLOCK_WIDTH * (block._pos.x + pos.x);
		int curr_y = BOARD_OFFSET_Y + BLOCK_HEIGHT * (block._pos.y + pos.y);
		Position curr_pos = { curr_x, curr_y };

		display_block(curr_pos, color);
	}
}

void View::display_blocks_and_board(const vector< vector<int> > & static_positions) {
	for (int i = -1; i <= TOTAL_BLOCK_LENGTH; i++) {
		int curr_y = BOARD_OFFSET_Y + i * BLOCK_HEIGHT;

		for (int j = -1; j <= TOTAL_BLOCK_WIDTH; j++) {
			int curr_x = BOARD_OFFSET_X + j * BLOCK_WIDTH;
			Position curr_pos = { .x = curr_x, .y = curr_y };

			/* display outer walls */
			if (i == -1 || i == TOTAL_BLOCK_LENGTH || j == -1 || j == TOTAL_BLOCK_WIDTH) {
				display_brick_on_window(curr_pos);
			}

			/* display inner area */
			else {
				SDL_Color color = BLOCK_COLOR;   

				/* check if this square contains a part of static block (-1), and if so, use their pre-defined colors */
				if (static_positions[i][j] != -1) {
					color = Block::get_color_from_type(static_cast<BlockType>(static_positions[i][j]));
					display_block(curr_pos, color);
				}
				else {
					display_filled_rectangle( curr_pos, BLOCK_WIDTH, BLOCK_HEIGHT, color );
					display_rectangle_outline( curr_pos, BLOCK_WIDTH, BLOCK_HEIGHT, BLOCK_OUTLINE_COLOR );
				}
			}
		}
	}
}


void View::display_end_screen() {

	/* background */
	display_filled_rectangle( ORIGIN, LENGTH, WIDTH, BACKGROUND_COLOR );

	/* init screen text */
	display_text_on_window("Game over. Press spacebar to play again", END_TEXT_POS, INIT_SCREEN_COLOR);
}

void View::update_environment(int score, const vector< vector<int> > & static_positions, const Block & block) {

	/* background */
	display_filled_rectangle( ORIGIN , _length, _width, BACKGROUND_COLOR );

	/* score */
	display_text_on_window( "SCORE : " + std::to_string(score), SCORE_DISPLAY_POS, SCORE_COLOR );

	/* blocks and board */
	display_blocks_and_board(static_positions);

	if (_show_hint) {
		display_moving_block_final_pos(block, static_positions);
	}

	display_current_moving_block(block);

}


void View::display_rectangle_outline(Position pos, int length, int width, SDL_Color color)
{
	SDL_SetRenderDrawBlendMode( _renderer, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( _renderer, color.r, color.g, color.b, color.a );
	SDL_Rect destination = { .x = pos.x, .y = pos.y, .w = width, .h = length };
	SDL_RenderDrawRect( _renderer, &destination );
}

void View::display_filled_rectangle(Position pos, int length, int width, SDL_Color color)
{
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a );
	SDL_Rect destination = { .x = pos.x, .y = pos.y, .w = width, .h = length };
	SDL_RenderFillRect(_renderer, &destination);
}

void View::display_text_on_window(string str_text, Position pos, SDL_Color color)
{
	const char * text = str_text.c_str();
	
	SDL_Surface* surface = TTF_RenderText_Blended(_font, text, color);
	CHECK_ERROR(surface == NULL, SDL_GetError());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
	CHECK_ERROR(texture == NULL, SDL_GetError());

	int length, width;
	SDL_QueryTexture(texture, NULL, NULL, &width, &length);
	SDL_Rect destination = { .x = pos.x, .y = pos.y, .w = width, .h = length };
	SDL_RenderCopy(_renderer, texture, nullptr, &destination);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void View::clear_renderer() {
	SDL_RenderClear(_renderer);
}

void View::present_renderer() {
	SDL_RenderPresent(_renderer);
}
