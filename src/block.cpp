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
    _block_type(_block_type),
    _pos(_pos),
    _rotation(_rotation)
    {
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