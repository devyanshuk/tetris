#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>
#include "position.h"
#include <vector>

/* window title */
inline const char * WINDOW_TITLE = "Tetris";


/* window dimensions */
inline const int WIDTH = 700;
inline const int LENGTH = 795;


/* grid dimensions */
inline const int TOTAL_BLOCK_WIDTH = 10;
inline const int TOTAL_BLOCK_LENGTH= 20;


/* block dimensions */
inline const int BLOCK_WIDTH = 36;
inline const int BLOCK_HEIGHT = 36;


/* tetris grid location */
inline const int BOARD_OFFSET_X = 280;
inline const int BOARD_OFFSET_Y = BLOCK_HEIGHT;

/* display positions */
inline const Position ORIGIN = { .x = 0, .y = 0 };
inline const Position SCORE_DISPLAY_POS = { .x = 4, .y = 200 };
inline const Position END_TEXT_POS = { .x = 20, .y = 350 };
inline const Position INIT_TEXT_POS = { .x = 130, .y = 350 };


/* block types */
enum BlockType {
	BLOCKTYPE_I = 0,
	BLOCKTYPE_J = 1,
	BLOCKTYPE_L = 2,
	BLOCKTYPE_O = 3,
	BLOCKTYPE_S = 4,
	BLOCKTYPE_T = 5,
	BLOCKTYPE_Z = 6,
};
inline const int NUM_BLOCKTYPES = 7;


/* block textures */
inline SDL_Color BLOCK_COLOR = { 255, 255, 255, 255 };
inline SDL_Color BLOCK_OUTLINE_COLOR = { 0, 0, 0, 255 };

/* block colors */
inline SDL_Color BLOCKTYPE_I_COLOR = { 4, 164, 255, 255 };
inline SDL_Color BLOCKTYPE_J_COLOR = { 233, 132, 43, 255 };
inline SDL_Color BLOCKTYPE_L_COLOR = { 255, 0, 0, 255 };
inline SDL_Color BLOCKTYPE_O_COLOR = { 216, 236, 35, 255 };
inline SDL_Color BLOCKTYPE_S_COLOR = { 180, 35, 216, 255 };
inline SDL_Color BLOCKTYPE_T_COLOR = { 0, 0, 255, 255 };
inline SDL_Color BLOCKTYPE_Z_COLOR = { 0, 255, 0, 255 };

/* text colors */
inline SDL_Color SCORE_COLOR = { 0, 255, 0, 50 };
inline SDL_Color INIT_SCREEN_COLOR = { 0, 0, 255, 200 };


/* background */
inline SDL_Color BACKGROUND_COLOR = { 0, 0, 0, 255 };


/* font */
inline const int FONT_SIZE = 30;
inline const char * FONT_LOCATION = "font.ttf";


/* wall image */
inline const char * WALL_IMAGE_LOCATION = "brick.png";


/* block spawn position */
inline const Position BLOCK_STARTING_POSITION = { .x = 4, .y = 0 };

/* block update speed */
inline const Uint32 VERTICAL_BLOCK_UPDATE_SPEED = 1000;
inline const Uint32 HORIZONTAL_BLOCK_UPDATE_SPEED = 120;


/* all positions of blocks */
inline const std::vector< std::vector<Position> > BLOCKTYPE_I_POSITIONS =
									{ { { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } },
									{ { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } },
									{ { 0, 2 }, { 1, 2 }, { 2, 2 }, { 3, 2 } },
									{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } } };

inline const std::vector< std::vector<Position> > BLOCKTYPE_J_POSITIONS =
									{ { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
									{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 1, 2 } },
									{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
									{ { 1, 0 }, { 1, 1 }, { 0, 2 }, { 1, 2 } } };

inline const std::vector< std::vector<Position> > BLOCKTYPE_L_POSITIONS =
									{ { { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
									{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
									{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 2 } },
									{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } } };

inline const std::vector< std::vector<Position> > BLOCKTYPE_O_POSITIONS =
									{ { { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
									{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
									{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
									{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } } };

inline const std::vector< std::vector<Position> > BLOCKTYPE_S_POSITIONS =
									{ { { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
									{ { 1, 0 }, { 1, 1 }, { 2, 1 }, { 2, 2 } },
									{ { 1, 1 }, { 2, 1 }, { 0, 2 }, { 1, 2 } },
									{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } } };

inline const std::vector< std::vector<Position> > BLOCKTYPE_T_POSITIONS =
									{ { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
									{ { 1, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
									{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } },
									{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } } };

inline const std::vector< std::vector<Position> > BLOCKTYPE_Z_POSITIONS =
									{ { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
									{ { 2, 0 }, { 1, 1 }, { 2, 1 },  { 1, 2 } },
									{ { 0, 1 }, { 1, 1 }, { 1, 2 }, { 2, 2 } },
									{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } } };

#endif
