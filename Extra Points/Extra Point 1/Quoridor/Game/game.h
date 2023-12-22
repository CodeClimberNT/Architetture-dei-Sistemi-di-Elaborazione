#ifndef GAME_FILE_INCLUDED  // Include guard
#define GAME_FILE_INCLUDED

#ifndef __STDINT_H
#include <stdint.h>
#endif


/*******************GAME CONFIGURATION**********************/

//BOARD OPTIONS
#define NUM_ROWS		7
#define NUM_COLUMNS	7
#define WALL_WIDTH 7
#define SQUARE_SIZE (MAX_X - (NUM_ROWS - 1) * WALL_WIDTH) / NUM_ROWS


//UI POSITION
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


//PLAYER OPTION
#define MAX_WALLS	8
#define MAX_TIME	20 //seconds
/*******************FUNCTION DEFINITION**********************/
//Utils Function
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

//LCD Extra Utils Function
void LCD_DrawRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void LCD_DrawSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color );

//Init Functions
void GameInit(void);
void BoardInit(void);
void UIInit(void);



#endif  // GAME_FILE_INCLUDED
