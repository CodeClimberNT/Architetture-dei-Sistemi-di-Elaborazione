#ifndef __GAME_H  // Include guard
#define __GAME_H

#ifndef __STDINT_H
#include "stdint.h"
#endif

#ifndef __GLCD_H 
#include "../GLCD/GLCD.h" 
#endif


void Setup(void);

/*******************CONSTANT DEFINITION**********************/
typedef enum {Vertical, Horizontal} WALL_DIRECTION;
typedef enum {UP, RIGHT, DOWN, LEFT, NORTH, EAST, SUD, OVEST} PLAYER_DIRECTION;

#define DarkGray 0x7BEF

/*******************UTILS FUNCTIONS**********************/
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/*******************LIBRARY LINKAGE**********************/
#include "init.h"
#include "player.h"
#include "board.h"
#include "draw_function.h"
#include "game_option.h"
#include "ui.h"








#endif  // __GAME_H
