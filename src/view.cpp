#include "../include/view.hpp"
#include <iostream>

View::View(const int & _width, const int & _length):
	intro_screen_prev_time(0),
	intro_text_opacity(255),
	intro_opacity_increasing(false)
{
    /* initialize sdl */
    CHECK_ERROR(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

    /* initialize window and renderer */
    SDL_CreateWindowAndRenderer(_width, _length, SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC, &this->_window, &this->_renderer);
    CHECK_ERROR(_window == NULL, SDL_GetError());
    CHECK_ERROR(_renderer == NULL, SDL_GetError());

    SDL_SetWindowTitle(_window, WINDOW_TITLE);

    /* initialize font */
    CHECK_ERROR(TTF_Init() == -1, TTF_GetError());
    this->_font = TTF_OpenFont(FONT_LOCATION, FONT_SIZE);
    CHECK_ERROR(_font == NULL, TTF_GetError());

	/* initialize image texture */
	CHECK_ERROR(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0, IMG_GetError());

	SDL_Surface * image = IMG_Load(WALL_IMAGE_LOCATION);
	CHECK_ERROR(image == NULL, IMG_GetError());

    this->_brick = SDL_CreateTextureFromSurface(this->_renderer, image);
	CHECK_ERROR(_brick == NULL, IMG_GetError());
    SDL_FreeSurface(image);

	this->_width = _width;
	this->_length = _length;
}

View::~View() 
{
	std::cout << "View destructor called" << std::endl;
    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
	SDL_DestroyTexture(this->_brick);
    TTF_CloseFont(this->_font);

	IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void View::init() {
	/* initialize intro screen animation helpers */
	intro_screen_prev_time = SDL_GetTicks();
	intro_text_opacity = 255;
	intro_opacity_increasing = false;
}

SDL_Color View::get_color_from_type(const BlockType & _type) {
	return _type == BLOCKTYPE_I ? BLOCKTYPE_I_COLOR :
		   _type == BLOCKTYPE_J ? BLOCKTYPE_J_COLOR :
		   _type == BLOCKTYPE_L ? BLOCKTYPE_L_COLOR :
		   _type == BLOCKTYPE_O ? BLOCKTYPE_O_COLOR :
		   _type == BLOCKTYPE_S ? BLOCKTYPE_S_COLOR :
		   _type == BLOCKTYPE_T ? BLOCKTYPE_T_COLOR :
		   BLOCKTYPE_Z_COLOR;
}

vector<Position> View::get_current_position(const BlockType & _type, const int & rotation) {
	return _type == BLOCKTYPE_I ? BLOCKTYPE_I_POSITIONS[rotation] :
		   _type == BLOCKTYPE_J ? BLOCKTYPE_J_POSITIONS[rotation] :
		   _type == BLOCKTYPE_L ? BLOCKTYPE_L_POSITIONS[rotation] :
		   _type == BLOCKTYPE_O ? BLOCKTYPE_O_POSITIONS[rotation] :
		   _type == BLOCKTYPE_S ? BLOCKTYPE_S_POSITIONS[rotation] :
		   _type == BLOCKTYPE_T ? BLOCKTYPE_T_POSITIONS[rotation] :
		   BLOCKTYPE_Z_POSITIONS[rotation];
}

void View::display_brick_on_window(const int & xpos, const int & ypos) {
	SDL_Rect destination = { xpos, ypos, BLOCK_WIDTH, BLOCK_HEIGHT };
	SDL_RenderCopy(this->_renderer, this->_brick, NULL, &destination);
}

void View::display_init_screen(const Uint32 & current_time) {

	/* background */
	this->display_filled_rectangle( 0, 0, this->_length, this->_width, BACKGROUND_COLOR );


	if (current_time > intro_screen_prev_time + 2) {
		intro_screen_prev_time = current_time;
		intro_text_opacity += intro_opacity_increasing ? 1 : -1;
		intro_opacity_increasing = (intro_text_opacity >= 255) ? false :
								   (intro_text_opacity <= 50) ? true :
								   intro_opacity_increasing;
	}

	SDL_Color init_screen_color = { .r = INIT_SCREEN_COLOR.r, .g = INIT_SCREEN_COLOR.g, .b = INIT_SCREEN_COLOR.b, .a = intro_text_opacity };

	/* init screen text */
	this->display_text_on_window("Press spacebar to continue", 130, 350, init_screen_color);
}

void View::display_current_moving_block(const Block & block) {
	std::vector<Position> display_pos = get_current_position(block.block_type, block.rotation);
	SDL_Color color = get_color_from_type(block.block_type);

	for (size_t i = 0; i < display_pos.size(); i++) {
		int curr_x = BOARD_OFFSET_X + BLOCK_WIDTH * (block.pos.x + display_pos[i].x);
		int curr_y = BOARD_OFFSET_Y + BLOCK_HEIGHT * (block.pos.y + display_pos[i].y);
		this->display_filled_rectangle( curr_x, curr_y, BLOCK_WIDTH, BLOCK_HEIGHT, color );
		this->display_rectangle_outline( curr_x, curr_y, BLOCK_WIDTH, BLOCK_HEIGHT, BLOCK_OUTLINE_COLOR );
	}
}


void View::display_end_screen() {

	/* background */
	this->display_filled_rectangle( 0, 0, LENGTH, WIDTH, BACKGROUND_COLOR );

	/* init screen text */
	this->display_text_on_window("Game over. Press spacebar to play again", 130, 350, INIT_SCREEN_COLOR);
}

void View::update_environment(const int & score, const vector< vector<int> > & static_positions, const Block & block) {

	/* background */
	this->display_filled_rectangle( 0, 0, this->_length, this->_width, BACKGROUND_COLOR );

	/* score */
	this->display_text_on_window( "SCORE : " + std::to_string(score), 4, 200, SCORE_COLOR );

	/* blocks and board */
	for (int i = -1; i <= TOTAL_BLOCK_LENGTH; i++) {
		int curr_y = BOARD_OFFSET_Y + i * BLOCK_HEIGHT;

		for (int j = -1; j <= TOTAL_BLOCK_WIDTH; j++) {
			int curr_x = BOARD_OFFSET_X + j * BLOCK_WIDTH;

			/* display outer walls */
			if (i == -1 || i == TOTAL_BLOCK_LENGTH || j == -1 || j == TOTAL_BLOCK_WIDTH) {
				this->display_brick_on_window( curr_x, curr_y );
			}

			/* display inner area */
			else {
				SDL_Color color = BLOCK_COLOR;

				/* check if this square contains a part of static block (-1), and if so, use their pre-defined colors */
				if (static_positions[i][j] != -1) {
					color = get_color_from_type(static_cast<BlockType>(static_positions[i][j]));
				}

				this->display_filled_rectangle( curr_x, curr_y, BLOCK_WIDTH, BLOCK_HEIGHT, color );
				this->display_rectangle_outline( curr_x, curr_y, BLOCK_WIDTH, BLOCK_HEIGHT, BLOCK_OUTLINE_COLOR );
			}
		}
	}

	this->display_current_moving_block(block);

}


void View::display_rectangle_outline(const int & xpos, const int & ypos, const int & length, const int & width, const SDL_Color & color)
{
	SDL_SetRenderDrawBlendMode( this->_renderer, SDL_BLENDMODE_BLEND );
	SDL_SetRenderDrawColor( this->_renderer, color.r, color.g, color.b, color.a );
    SDL_Rect rectangle = { .x = xpos, .y = ypos, .w = width, .h = length };
	SDL_RenderDrawRect( this->_renderer, &rectangle );
}

void View::display_filled_rectangle(const int & xpos, const int & ypos, const int & length, const int & width, const SDL_Color & color)
{
	SDL_SetRenderDrawBlendMode(this->_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a );
	SDL_Rect destination = { .x = xpos, .y = ypos, .w = width, .h = length };
	SDL_RenderFillRect(this->_renderer, &destination);
}

void View::display_text_on_window(const string & str_text, const int & xpos, const int & ypos, const SDL_Color & color)
{
	const char * text = str_text.c_str();

	SDL_Surface* surface = TTF_RenderText_Blended(_font, text, color);
	CHECK_ERROR(surface == NULL, SDL_GetError());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
	CHECK_ERROR(texture == NULL, SDL_GetError());

	int length, width;
	SDL_QueryTexture(texture, NULL, NULL, &width, &length);
	SDL_Rect destination = { .x = xpos, .y = ypos, .w = width, .h = length };
	SDL_RenderCopy(this->_renderer, texture, nullptr, &destination);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void View::clear_renderer() {
    SDL_RenderClear(this->_renderer);
}

void View::present() {
    SDL_RenderPresent(this->_renderer);
}