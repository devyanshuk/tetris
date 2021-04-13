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
	_view = std::make_unique<View>(input._width, input._length, input._show_hint);
	_view->init_animation_fields();
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
		std::vector<std::optional<int> > row(TOTAL_BLOCK_WIDTH, std::nullopt);
		_non_moving_blocks.push_back(row);
	}
}

bool Tetris::make_new_block() {
	BlockType new_block_type = static_cast<BlockType>(rand() % NUM_BLOCKTYPES);
	Block check_block(new_block_type);
	if (Block::is_collision(check_block, _non_moving_blocks)) {
		return false;
	}
	_current_active_block = check_block;
	return true;
}

bool Tetris::try_moving_piece(Block test_block) {
	return !Block::is_collision(test_block, _non_moving_blocks);
}

void Tetris::remove_all_complete_rows() {
	int lines_cleared = 0;
	int clear_from = 0;
	for (int i = TOTAL_BLOCK_LENGTH - 1; i >= 0; i--) {
		std::vector<std::optional<int>> current_row = _non_moving_blocks[i];
		if (std::find(current_row.begin(), current_row.end(), std::nullopt) == current_row.end()) {
			lines_cleared++;
			if (clear_from < i) {
				clear_from = i;
			}
		}
	}
	if (lines_cleared == 0) {
		return;
	}
	std::vector<std::optional<int> > empty_row(TOTAL_BLOCK_WIDTH, std::nullopt);
	for (int i = clear_from - lines_cleared; i >= 0; i--) {
		_non_moving_blocks[i + lines_cleared] = _non_moving_blocks[i];
	}
	_non_moving_blocks[0] = empty_row;
}

/* returns true if the block became static */
bool Tetris::check_collision(int new_x_pos, int new_y_pos, int new_rotation) {
	Position possible_new_pos = { .x = new_x_pos, .y = new_y_pos };
	Block test_block(_current_active_block._block_type, possible_new_pos, new_rotation);
	if (try_moving_piece(test_block)) {
		_current_active_block = test_block;
	}
	/* if there is a collision after moving the block vertically, that block will be static */
	else if (new_y_pos != _current_active_block._pos.y) {
		std::vector<Position> all_current_block_positions = Block::get_current_position(_current_active_block._block_type, new_rotation);
		for (const Position & pos : all_current_block_positions ) {
			int x = pos.x + _current_active_block._pos.x;
			int y = pos.y + _current_active_block._pos.y;

			_non_moving_blocks[y][x] = _current_active_block._block_type;
		}
		remove_all_complete_rows();
		return true;
	}
	return false;
}

void Tetris::update_game_state() {
	if (*_key == SDLK_SPACE) {
		if (_state == GAMESTATE_INIT_SCREEN) {
			_state = GAMESTATE_PLAYING;
			init();
		}
		else if (_state == GAMESTATE_END) {
			_state = GAMESTATE_INIT_SCREEN;
			_view->init_animation_fields();
			_key = {};
		}
	}
}

bool Tetris::update_game() {
	if (_state != GAMESTATE_PLAYING) {
		_prev_y_update_time = SDL_GetTicks();
		_prev_x_update_time = SDL_GetTicks();
		update_game_state();
		return true;
	}

	if (SDL_GetTicks() - _prev_x_update_time >= HORIZONTAL_BLOCK_UPDATE_SPEED) {
		_prev_x_update_time = SDL_GetTicks();
		size_t active_blocks;
		Block block;

		switch(*_key) {

			case SDLK_LEFT:
				check_collision(_current_active_block._pos.x - 1, _current_active_block._pos.y, _current_active_block._rotation);
				_key = {};
				break;

			case SDLK_RIGHT:
				check_collision(_current_active_block._pos.x + 1, _current_active_block._pos.y, _current_active_block._rotation);
				_key = {};
				break;

			case SDLK_x:
				active_blocks = Block::get_current_position(_current_active_block._block_type, _current_active_block._rotation).size();
				check_collision(_current_active_block._pos.x, _current_active_block._pos.y, (_current_active_block._rotation + 1) % active_blocks);
				_key = {};
				break;

			case SDLK_z:
				active_blocks = Block::get_current_position(_current_active_block._block_type, _current_active_block._rotation).size();
				check_collision(_current_active_block._pos.x, _current_active_block._pos.y, (_current_active_block._rotation - 1) % active_blocks);
				_key = {};
				break;

			case SDLK_UP:
				block = _current_active_block;
				while (try_moving_piece(block)) {
					block._pos.y++;
				}
				block._pos.y--;
				check_collision(block._pos.x, block._pos.y, block._rotation);
				_key = {};
				break;
		}
	}

	if (get_tick_difference() >= _block_vertical_update_speed) {
		_prev_y_update_time = SDL_GetTicks();

		/* move down a block every tick */
		if (check_collision(_current_active_block._pos.x, _current_active_block._pos.y + 1, _current_active_block._rotation)) {
			return make_new_block();
		}
	}

	else if (*_key == SDLK_DOWN) {
		if (check_collision(_current_active_block._pos.x, _current_active_block._pos.y + 1, _current_active_block._rotation)) {
			return make_new_block();
		}
		_key = {};
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

	while (game_running) {

		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				game_running = false;
			}
			else if(event.type == SDL_KEYDOWN) {
				_key = event.key.keysym.sym;
				switch(*_key) {

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
			if (!update_game()) {
				_key = {};
				_state = GAMESTATE_END;
			}
			_view->clear_renderer();
			if (!update_screen()){
				break;
			}
			_view->present_renderer();
			SDL_Delay(1);
		}
	}
	return 0;
}
