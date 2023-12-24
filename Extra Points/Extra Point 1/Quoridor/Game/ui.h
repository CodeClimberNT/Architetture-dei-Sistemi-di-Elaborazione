#ifndef __UI_H  // Include guard
#define __UI_H


// Starting UI
#define READY_UI_WIDTH  MAX_X * 0.8
#define READY_UI_HEIGHT 80

#define READY_UI_XPOS  (MAX_X - READY_UI_WIDTH)/2 	//CENTER HORIZONTAL
#define READY_UI_YPOS  (MAX_Y - READY_UI_HEIGHT)/2  //CENTER VERTICAL



//Height for game ui
#define UI_HEIGHT 2*SQUARE_SIZE

#define UI_YPOS (SQUARE_SIZE + WALL_WIDTH) * NUM_ROWS + SQUARE_SIZE/3

//Width for player and timer
#define P_UI_WIDTH 64
#define T_UI_WIDTH 72

//Pos for players ui
#define P1_UI_XPOS 10
#define P1_UI_YPOS UI_YPOS
#define P2_UI_XPOS MAX_X - 10 - P_UI_WIDTH
#define P2_UI_YPOS UI_YPOS

//Pos for timer ui
#define T_UI_XPOS (P1_UI_XPOS + P_UI_WIDTH + 10)
#define T_UI_YPOS	UI_YPOS

#define T_UI_HEIGHT 70


#endif  // __UI_H
