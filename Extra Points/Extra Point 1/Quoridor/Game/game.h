#ifndef __GAME_H  // Include guard
#define __GAME_H

#ifndef __STDINT_H
#include "stdint.h"
#endif

#ifndef __MY_STDIO_H
#define __MY_STDIO_H
#include "stdio.h"
#endif
/*******************PERIPHERAL LINKAGE**********************/
#ifndef __MY_PERIPHERAL_H
#define __MY_PERIPHERAL_H
#include "../timer/timer.h"
#include "../button_EXINT/button.h"
#include "../RIT/RIT.h"
#include "../joystick/joystick.h"
#include "../GLCD/GLCD.h" 
#endif


/*******************CONSTANT DEFINITION**********************/
typedef enum {Vertical, Horizontal} WALL_DIRECTION;
typedef enum {UP, RIGHT, DOWN, LEFT} DIRECTION;
typedef enum {NORTH, NORTH_EAST, EAST, SUD_EAST, SUD, SUD_OVEST, OVEST, NORTH_OVEST, OVEST_NORTH_EAST} SHADOW_DIRECTION;

/*******************COLOR DEFINITION**********************/
#define GameBG White
#define DarkGray 0x7BEF
#define ChristmasRed 0xF800
#define ChristmasGreen 0xE60
#define QuoridorRed 0xA9A9

/*******************UTILS FUNCTIONS**********************/
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/*******************GAME FUNCTION**********************/
void PeripheralInit(void);
void Setup(void);
void Waiting_Player(void);
void Position_Wall(uint8_t x, uint8_t y, WALL_DIRECTION wall_dir);

/*******************LIBRARY LINKAGE**********************/
#include "init.h"
#include "player.h"
#include "board.h"
#include "draw_function.h"
#include "game_option.h"
#include "ui.h"


#endif  // __GAME_H
