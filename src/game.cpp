#include "../include/game.hpp"
#include <iostream>

using namespace std;

Tetris::Tetris(ArgInput input):
_score(0),
_state(GAMESTATE_INIT_SCREEN),
_block_update_speed(BLOCK_UPDATE_SPEED)
{
	/* seed the pseudo random number generator */
	srand((unsigned int)time(NULL));
	_prev_time = SDL_GetTicks();
	_view = std::make_unique<View>(input._width, input._length);
	_view->init_animation_fields();
}

Tetris::~Tetris() {
	std::cout << "game destructed" << std::endl;
}

bool Tetris::init() {
	_score = 0;
	_view->init_animation_fields();
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

void Tetris::check_collision(int new_x_pos, int new_y_pos) {
	Position possible_new_pos = { .x = new_x_pos, .y = new_y_pos };
	Block test_block(_current_active_block._block_type, possible_new_pos);
	if (try_moving_piece(test_block)) {
		_current_active_block = test_block;
	}
}

void Tetris::update_game(const SDL_Keycode & key, const Uint32 & delta_time) {
	if (delta_time >= _block_update_speed) {
		_prev_time = SDL_GetTicks();

		switch(key) {

			case SDLK_LEFT:
				check_collision(_current_active_block._pos.x - 1, _current_active_block._pos.y);
				break;

			case SDLK_RIGHT:
				check_collision(_current_active_block._pos.x + 1, _current_active_block._pos.y);
				break;
		}

		/* move down a block every tick */
		check_collision(_current_active_block._pos.x, _current_active_block._pos.y + 1);

	}
}

bool Tetris::update_screen(const SDL_Keycode & key, const Uint32 & delta_time) {
	switch (_state) {

		case GAMESTATE_INIT_SCREEN:
		_view->display_init_screen(SDL_GetTicks());
		if (key == SDLK_SPACE) {
			_state = GAMESTATE_PLAYING;
			if (!init()) {
				return false;
			}
		}
		break;

		case GAMESTATE_PLAYING:
		_view->update_environment(_score, _non_moving_blocks, _current_active_block);
		update_game(key, delta_time);
		break;

		case GAMESTATE_END:
		_view->display_end_screen();
		if (key == SDLK_SPACE) {
			_state = GAMESTATE_INIT_SCREEN;
			_view->init_animation_fields();
		}
		break;
	}
	return true;
}

Uint32 Tetris::get_tick_difference() {
	Uint32 curr_time = SDL_GetTicks();
	Uint32 delta_time = curr_time - _prev_time;
	return delta_time;
}

int Tetris::play() {
	SDL_Event event;
	bool game_running = true;
	bool game_pause = false;

	while (game_running) {
		Uint32 delta_time = get_tick_difference();
		SDL_Keycode key;

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
				}
			}
		}
		if (!game_pause) {
			_view->clear_renderer();
			if (!update_screen(key, delta_time)){
				break;
			}
			_view->present_renderer();
		}
	}
	return 0;
}
