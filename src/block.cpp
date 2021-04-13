#include "../include/block.hpp"

Block::Block(BlockType _block_type): _pos (BLOCK_STARTING_POSITION), _rotation (0)
{
	this->_block_type = _block_type;
}

Block::Block(BlockType _block_type, Position _pos) : _rotation (0)
{
	this->_block_type = _block_type;
	this->_pos = _pos;
}

Block::Block(): _pos (BLOCK_STARTING_POSITION), _rotation (0)
{
}

Block & Block::operator=(const Block & other) {
	this->_rotation = other._rotation;
	this->_block_type = other._block_type;
	this->_pos = other._pos;
	return *this;
}

Block::Block(BlockType _block_type, Position _pos, int _rotation):
_pos(_pos),
_block_type(_block_type),
_rotation(_rotation)
{
}

bool Block::is_collision(const Block & test_block, std::vector<std::vector<std::optional<int> > > _non_moving_blocks) {
	std::vector<Position> pos = get_current_position(test_block._block_type, test_block._rotation);
	for (size_t i = 0; i < pos.size(); i++) {

		/* translate x and y positions by test block x and y positions */
		int curr_x = pos[i].x + test_block._pos.x;
		int curr_y = pos[i].y + test_block._pos.y;

		/* if there already was a block in the grid, return false */
		if (curr_x < 0 ||
			curr_x >= TOTAL_BLOCK_WIDTH ||
			curr_y < 0 ||
			curr_y >= TOTAL_BLOCK_LENGTH ||
			_non_moving_blocks[curr_y][curr_x].has_value())
			{
				return true;
			}
		}
		return false;
	}

std::vector<Position> Block::get_current_position(const BlockType & _type, const int & rotation) {
	return _type == BLOCKTYPE_I ? BLOCKTYPE_I_POSITIONS[rotation] :
	_type == BLOCKTYPE_J ? BLOCKTYPE_J_POSITIONS[rotation] :
	_type == BLOCKTYPE_L ? BLOCKTYPE_L_POSITIONS[rotation] :
	_type == BLOCKTYPE_O ? BLOCKTYPE_O_POSITIONS[rotation] :
	_type == BLOCKTYPE_S ? BLOCKTYPE_S_POSITIONS[rotation] :
	_type == BLOCKTYPE_T ? BLOCKTYPE_T_POSITIONS[rotation] :
	BLOCKTYPE_Z_POSITIONS[rotation];
}

SDL_Color Block::get_color_from_type(const BlockType & _type) {
	return _type == BLOCKTYPE_I ? BLOCKTYPE_I_COLOR :
	_type == BLOCKTYPE_J ? BLOCKTYPE_J_COLOR :
	_type == BLOCKTYPE_L ? BLOCKTYPE_L_COLOR :
	_type == BLOCKTYPE_O ? BLOCKTYPE_O_COLOR :
	_type == BLOCKTYPE_S ? BLOCKTYPE_S_COLOR :
	_type == BLOCKTYPE_T ? BLOCKTYPE_T_COLOR :
	BLOCKTYPE_Z_COLOR;
}
