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
}

Tetris::~Tetris() {
	std::cout << "game destructed" << std::endl;
}

int Tetris::init() {
	_score = 0;
	_view->init();
	init_static_blocks();
	make_new_block();
	return 1;
}

void Tetris::init_static_blocks() {
	for (int i = 0; i < TOTAL_BLOCK_LENGTH; i++) {
		std::vector<int> row;
		for (int j = 0; j < TOTAL_BLOCK_WIDTH; j++) {
			row.push_back(-1);
		}
		_static_blocks.push_back(row);
	}
}

bool Tetris::make_new_block() {
	int _type = rand() % NUM_BLOCKTYPES;
	BlockType new_block_type = static_cast<BlockType>(_type);
	_current_active_block.pos = BLOCK_STARTING_POSITION;
	_current_active_block.block_type = new_block_type;
	_current_active_block.rotation = 0;
	return true;
}

void Tetris::update_game(const Uint32 & delta_time) {
	if (delta_time >= _block_update_speed && _current_active_block.pos.y + 2 < TOTAL_BLOCK_LENGTH) {
		_prev_time = SDL_GetTicks();
		_current_active_block.pos.y++;
	}
}

void Tetris::update_screen(const SDL_Keycode & key, const Uint32 & delta_time) {

	switch (_state) {

		case GAMESTATE_INIT_SCREEN:
		_view->display_init_screen(SDL_GetTicks());
		if (key == SDLK_SPACE) {
			_state = GAMESTATE_PLAYING;
			init();
		}
		break;

		case GAMESTATE_PLAYING:
		_view->update_environment(_score, _static_blocks, _current_active_block);
		update_game(delta_time);
		break;

		case GAMESTATE_END:
		_view->display_end_screen();
		if (key == SDLK_SPACE) {
			_state = GAMESTATE_INIT_SCREEN;
			_view->init();
		}
		break;
	}
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
			update_screen(key, delta_time);
			_view->present();
		}
	}
	return true;
}
