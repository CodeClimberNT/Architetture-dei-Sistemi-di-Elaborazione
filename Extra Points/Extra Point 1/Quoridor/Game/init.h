#ifndef MY_GAME_FILE_INCLUDED  // Include guard
#define MY_GAME_FILE_INCLUDED

#ifndef __STDINT_H
#include <stdint.h>
#endif


//BOARD OPTIONS
#define NUM_ROWS		7
#define NUM_COLUMNS	7
#define WALL_WIDTH 7
#define SQUARE_SIZE (MAX_X - (NUM_ROWS - 1) * WALL_WIDTH) / NUM_ROWS


//UI OPTIONS
#define P1_UI_XPOS 15
#define P1_UI_YPOS 250
#define P2_UI_XPOS 144
#define P2_UI_YPOS 250

#define P_UI_WIDTH 80
#define P_UI_HEIGHT 60

#define T_UI_XPOS  (P1_UI_XPOS + P_UI_WIDTH + 10)
#define T_UI_YPOS	 260
#define T_UI_WIDTH 80
#define T_UI_HEIGHT 40

//Utils Function
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))


void GameInit(void);
void BoardInit(void);
void UIInit(void);
#endif  // MY_GAME_FILE_INCLUDED
