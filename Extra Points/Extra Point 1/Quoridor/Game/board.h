#ifndef __BOARD_H  // Include guard
#define __BOARD_H



#define NUM_ROWS		7
#define NUM_COLUMNS	7
#define WALL_WIDTH  6 
#define SQUARE_SIZE (MAX_X - (NUM_ROWS - 1) * WALL_WIDTH) / NUM_ROWS
//wall takes two square plus the wall width in the interception
#define WALL_LENGTH (SQUARE_SIZE * 2 + WALL_WIDTH)

#endif  // __BOARD_H
