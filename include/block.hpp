#pragma once

#include "position.h"
#include "constants.hpp"

#include <vector>
#include <SDL2/SDL.h>

class Block {

	public:

		Position 	_pos;
		BlockType 	_block_type;
		int 		_rotation;

		Block(BlockType block_type);
		Block(BlockType block_type, Position pos);
		Block(BlockType block_type, Position pos, int _rotation);
		Block();

		Block & operator = (const Block & other);
		static SDL_Color get_color_from_type(const BlockType & _type);
		static std::vector<Position> get_current_position(const BlockType & _type, const int & rotation);
};
