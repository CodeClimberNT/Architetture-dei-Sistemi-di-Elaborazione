#ifndef __GAME_CONSTANT_H
#define __GAME_CONSTANT_H


typedef enum { UP,
               RIGHT,
               DOWN,
               LEFT } DIRECTION;

							 

#define WALL_DISCOUNT 1

// PLAYER OPTIONS
#define MAX_WALLS 8
#define MAX_TIME 20  // seconds
#define COOLDOWN 2   // wait #COOLDOWN before allowed to next move

// BOARD OPTIONS
#define NUM_ROWS 7
#define NUM_COLUMNS 7
#define NUM_ROWS_WALL NUM_ROWS - 1
#define NUM_COLUMNS_WALL NUM_COLUMNS - 1
#define WALL_WIDTH 6
#define HALF_WALL_WIDTH WALL_WIDTH / 2
#define SQUARE_SIZE (MAX_X - (NUM_ROWS - 1) * WALL_WIDTH) / NUM_ROWS
#define PLAYER_SIZE SQUARE_SIZE - 2
#define WALL_LENGTH (SQUARE_SIZE * 2 + WALL_WIDTH)  // wall takes two square plus the wall width in the interception

// UI OPTIONS

// READY
#define READY_UI_WIDTH MAX_X * 0.8
#define READY_UI_HEIGHT 80

#define READY_UI_XPOS (MAX_X - READY_UI_WIDTH) / 2   // CENTER HORIZONTALLY
#define READY_UI_YPOS (MAX_Y - READY_UI_HEIGHT) / 2  // CENTER VERTICALLY

// HEIGHT DURING GAME
#define UI_HEIGHT 2 * SQUARE_SIZE

#define UI_YPOS (SQUARE_SIZE + WALL_WIDTH) * NUM_ROWS + SQUARE_SIZE / 3

// PLAYER UI
#define P_UI_WIDTH 64

#define P0_UI_XPOS 10
#define P0_UI_YPOS UI_YPOS
#define P1_UI_XPOS MAX_X - 10 - P_UI_WIDTH
#define P1_UI_YPOS UI_YPOS

// Pos for timer ui
#define T_UI_XPOS (P0_UI_XPOS + P_UI_WIDTH + 5)
#define T_UI_YPOS UI_YPOS

#define T_UI_HEIGHT 70
#define T_UI_WIDTH 82


#endif 
