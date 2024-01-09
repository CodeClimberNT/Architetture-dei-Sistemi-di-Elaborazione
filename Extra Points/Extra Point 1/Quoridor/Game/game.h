#ifndef __GAME_H  // Include guard
#define __GAME_H

#ifndef __MY_STDINT_H
#define __MY_STDINT_H
#include "stdint.h"
#endif

#ifndef __MY_STDIO_H
#define __MY_STDIO_H
#include "stdio.h"
#endif

/*******************LIBRARY LINKAGE**********************/
#ifndef __MY_LIBRARY_H
#define __MY_LIBRARY_H
#include "../Draw/draw.h"
#include "../Player/player.h"
#include "../UI/ui.h"
#include "../Wall/wall.h"
#endif

/*******************PERIPHERAL LINKAGE**********************/
#ifndef __MY_PERIPHERAL_H
#define __MY_PERIPHERAL_H
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"
#include "../button_EXINT/button.h"
#include "../joystick/joystick.h"
#include "../timer/timer.h"
#endif

/*******************CONSTANT DEFINITION**********************/

// READY
#define TITLE_UI_WIDTH MAX_X * 0.8
#define TITLE_UI_HEIGHT 80

#define TITLE_UI_XPOS (MAX_X - TITLE_UI_WIDTH) / 2   // CENTER HORIZONTALLY
#define TITLE_UI_YPOS (MAX_Y - TITLE_UI_HEIGHT) / 2  // CENTER VERTICALLY

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

// PLAYER OPTIONS
#define MAX_WALLS 8
#define MAX_TIME 20  // seconds
#define COOLDOWN 2   // wait #COOLDOWN before allowed to next move

// BOARD OPTIONS
#define NUM_ROWS 7
#define NUM_COLUMNS 7
#define NUM_ROWS_WALL NUM_ROWS - 1
#define NUM_COLUMNS_WALL NUM_COLUMNS - 1

#define SQUARE_SIZE (MAX_X - (NUM_ROWS - 1) * WALL_WIDTH) / NUM_ROWS
#define PLAYER_SIZE SQUARE_SIZE - 2
#define WALL_LENGTH (SQUARE_SIZE * 2 + WALL_WIDTH)  // wall takes two square plus the wall width in the interception

/*************ENUM**************/
typedef enum { TRANSITION,
               IDLE } GAME_STATE;
typedef enum { PLAYER,
               WALL } MOVING_ENTITY;

/*******************UTILS FUNCTIONS**********************/
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MOD(x, y) ((x % y + y) % y)

void wait_delay(int count);

/*******************GAME FUNCTION**********************/
void Setup(void);

void Start_Game(void);

// INIT FUNCTION
void Peripheral_Init(void);
void Player_Init(void);
void Board_Init(void);
void UI_Counter_Init(void);
void UI_Init(void);

// GAME MODE FUNCTION
void Title_Screen(void);
void End_Turn(uint8_t timeout);
void Timer_End_Turn(void);

uint8_t Win_Condition(void);
void Game_Over(void);

void Switch_Player_Wall(void);

void Draw_Board(void);
void Draw_Checkers(void);

uint8_t Wall_Partition_Board(struct Wall m_wall);
uint8_t isReachable(struct Vector2D start, uint8_t row_target, uint8_t tempBoard[NUM_COLUMNS + NUM_COLUMNS_WALL][NUM_ROWS + NUM_ROWS_WALL]);

void Move(DIRECTION dir);

#endif
