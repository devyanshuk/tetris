#pragma once

#include "position.h"
#include "constants.hpp"

class TetrisBlock {

    public:
        TetrisBlock();
        ~TetrisBlock();

        BlockType block;
        Position pos;

    private:

};