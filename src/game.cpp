#include "../include/game.hpp"
#include <iostream>

using namespace std;

Tetris::Tetris(ArgInput input):
_score(0),
_state(GAMESTATE_INIT_SCREEN),
_block_vertical_update_speed(VERTICAL_BLOCK_UPDATE_SPEED)
{
	/* seed the pseudo random number generator */
	srand((unsigned int)time(NULL));
	_prev_y_update_time = SDL_GetTicks();
	_prev_x_update_time = SDL_GetTicks();
	_view = std::make_unique<View>(input._width, input._length);
	_view->init_animation_fields();
}

Tetris::~Tetris() {
	std::cout << "game destructed" << std::endl;
}

bool Tetris::init() {
	_score = 0;
	_view->init_animation_fields();
	_non_moving_blocks.clear();
	init_non_moving_blocks();
	if (!make_new_block()) {
		return false;
	}
	return true;
}

void Tetris::init_non_moving_blocks() {
	for (int i = 0; i < TOTAL_BLOCK_LENGTH; i++) {
		std::vector<int> row;
		for (int j = 0; j < TOTAL_BLOCK_WIDTH; j++) {
			row.push_back(-1);
		}
		_non_moving_blocks.push_back(row);
	}
}

bool Tetris::is_collision(const Block & test_block) {
	std::vector<Position> all_test_block_positions = Block::get_current_position(test_block._block_type, test_block._rotation);
	for (size_t i = 0; i < all_test_block_positions.size(); i++) {

		/* translate x and y positions by current block x and y positions */
		int curr_x = all_test_block_positions[i].x + test_block._pos.x;
		int curr_y = all_test_block_positions[i].y + test_block._pos.y;

		/* if there already was a block in the grid, return false */
		if (curr_x < 0 ||
			curr_x >= TOTAL_BLOCK_WIDTH ||
			curr_y < 0 ||
			curr_y >= TOTAL_BLOCK_LENGTH ||
			_non_moving_blocks[curr_y][curr_x] != -1)
			{
				return true;
			}
	}
	return false;
}

bool Tetris::make_new_block() {
	int _type = rand() % NUM_BLOCKTYPES;
	BlockType new_block_type = static_cast<BlockType>(_type);
	Block check_block(new_block_type);
	if (is_collision(check_block)) {
		return false;
	}
	_current_active_block = check_block;
	return true;
}

bool Tetris::try_moving_piece(Block test_block) {
	return !is_collision(test_block);
}

/* returns true if the block became static */
bool Tetris::check_collision(int new_x_pos, int new_y_pos) {
	Position possible_new_pos = { .x = new_x_pos, .y = new_y_pos };
	Block test_block(_current_active_block._block_type, possible_new_pos);
	if (try_moving_piece(test_block)) {
		_current_active_block = test_block;
	}
	/* if there is a collision after moving the block vertically, that block will be static */
	else if (new_y_pos != _current_active_block._pos.y) {
		std::vector<Position> all_current_block_positions = Block::get_current_position(_current_active_block._block_type, _current_active_block._rotation);
		for (size_t i = 0; i < all_current_block_positions.size(); i++) {
			int x = all_current_block_positions[i].x + _current_active_block._pos.x;
			int y = all_current_block_positions[i].y + _current_active_block._pos.y;

			_non_moving_blocks[y][x] = _current_active_block._block_type;
		}
		return true;
	}
	return false;
}

void Tetris::update_game_state(SDL_Keycode key) {
	if (key == SDLK_SPACE) {
		if (_state == GAMESTATE_INIT_SCREEN) {
			_state = GAMESTATE_PLAYING;
			init();
		}
		else if (_state == GAMESTATE_END) {
			_state = GAMESTATE_INIT_SCREEN;
			_view->init_animation_fields();
		}
	}
}

bool Tetris::update_game(SDL_Keycode & key) {
	if (_state != GAMESTATE_PLAYING) {
		_prev_y_update_time = SDL_GetTicks();
		_prev_x_update_time = SDL_GetTicks();
		update_game_state(key);
		return true;
	}

	if (SDL_GetTicks() - _prev_x_update_time >= HORIZONTAL_BLOCK_UPDATE_SPEED) {
		_prev_x_update_time = SDL_GetTicks();
		switch(key) {

			case SDLK_LEFT:
				check_collision(_current_active_block._pos.x - 1, _current_active_block._pos.y);
				break;

			case SDLK_RIGHT:
				check_collision(_current_active_block._pos.x + 1, _current_active_block._pos.y);
				break;

			default:
				goto mov_y;
		}
		key = SDLK_0;
	}

mov_y:
	if (get_tick_difference() >= _block_vertical_update_speed) {
		_prev_y_update_time = SDL_GetTicks();

		/* move down a block every tick */
		if (check_collision(_current_active_block._pos.x, _current_active_block._pos.y + 1)) {
			return make_new_block();
		}
	}
	return true;
}

bool Tetris::update_screen() {
	switch (_state) {

		case GAMESTATE_INIT_SCREEN:
			_view->display_init_screen(SDL_GetTicks());
			break;

		case GAMESTATE_PLAYING:
			_view->update_environment(_score, _non_moving_blocks, _current_active_block);
			break;

		case GAMESTATE_END:
			_view->display_end_screen();
			break;
	}
	return true;
}

Uint32 Tetris::get_tick_difference() {
	return SDL_GetTicks() - _prev_y_update_time;
}

int Tetris::play() {
	SDL_Event event;
	bool game_running = true;
	bool game_pause = false;
	SDL_Keycode key = SDLK_0; /* dummy value */

	while (game_running) {

		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				game_running = false;
			}
			else if(event.type == SDL_KEYDOWN) {
				key = event.key.keysym.sym;
				switch(key) {

					case SDLK_ESCAPE:
						game_running = false;
						break;

					case SDLK_p:
						game_pause = !game_pause;
						break;

					default:
						game_pause = false;
						break;
				}
			}
		}
		if (!game_pause) {
			if (!update_game(key)) {
				key = SDLK_0;
				_state = GAMESTATE_END;
			}
			_view->clear_renderer();
			if (!update_screen()){
				break;
			}
			_view->present_renderer();
		}
	}
	return 0;
}
