#include "../include/game.hpp"
#include <iostream>

using namespace std;

Tetris::Tetris():
    _width(WIDTH),
    _length(LENGTH),
    _score(0),
    _state(GAMESTATE_INIT_SCREEN),
    _block_update_speed(BLOCK_UPDATE_SPEED)
{
    srand((unsigned int)time(NULL));
    _prev_time = SDL_GetTicks();
    _view = nullptr;
}

void Tetris::init_view() {
    this->_view = new View(this->_width, this->_length);
}

int Tetris::init() {
    /* seed the pseudo random number generator */
    this->_score = 0;
    this->_view->init();
    this->init_static_blocks();
    this->make_new_block();
    return 1;
}

void Tetris::init_static_blocks() {
    for (int i = 0; i < TOTAL_BLOCK_LENGTH; i++) {
        std::vector<int> row;
        for (int j = 0; j < TOTAL_BLOCK_WIDTH; j++) {
            row.push_back(-1);
        }
        this->_static_blocks.push_back(row);
    }
}

bool Tetris::make_new_block() {
    int _type = rand() % NUM_BLOCKTYPES;
    cout << "TYPE RANDOM = " << _type << endl;
    BlockType new_block_type = static_cast<BlockType>(_type);
    this->_current_active_block.pos = BLOCK_STARTING_POSITION;
    this->_current_active_block.block_type = new_block_type;
    this->_current_active_block.rotation = 0;
    return true;
}

void Tetris::update_game(const Uint32 & delta_time) {
    if (delta_time >= this->_block_update_speed && this->_current_active_block.pos.y + 2 < TOTAL_BLOCK_LENGTH) {
        this->_prev_time = SDL_GetTicks();
        this->_current_active_block.pos.y++;
    }
}

void Tetris::update_screen(const SDL_Keycode & key, const Uint32 & delta_time) {

    switch (this->_state) {

        case GAMESTATE_INIT_SCREEN:
            this->_view->display_init_screen(SDL_GetTicks());
            if (key == SDLK_SPACE) {
                this->_state = GAMESTATE_PLAYING;
                this->init();
                cout << "here" << endl;
            }
            break;
        
        case GAMESTATE_PLAYING:
            this->_view->update_environment(this->_score, this->_static_blocks, this->_current_active_block);
            this->update_game(delta_time);
            break;

        case GAMESTATE_END:
            this->_view->display_end_screen();
            if (key == SDLK_SPACE) {
                this->_state = GAMESTATE_INIT_SCREEN;
                this->_view->init();
            }
            break;
    }
}

int Tetris::get_score() {
    return this->_score;
}

Uint32 Tetris::get_tick_difference() {
    Uint32 curr_time = SDL_GetTicks();
    Uint32 delta_time = curr_time - this->_prev_time;
    return delta_time;
}

void Tetris::play() {
    SDL_Event event;
    bool game_running = true;
    bool game_pause = false;

    while (game_running) {
        Uint32 delta_time = this->get_tick_difference();
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
                        game_pause = true;
                        break;
                
                    default:
                        game_pause = false;
                }
            }
        }
        if (!game_pause) {
            this->_view->clear_renderer();
            update_screen(key, delta_time);
            this->_view->present();
        }
    }
}

/* overwrite the game width and length, before initialization */

void Tetris::set_length(int _length) {
    this->_length = _length;
}

void Tetris::set_width(int _width) {
    this->_width = _width;
}

Tetris::~Tetris() {
    if (this->_view) {
        delete this->_view;
    }
}