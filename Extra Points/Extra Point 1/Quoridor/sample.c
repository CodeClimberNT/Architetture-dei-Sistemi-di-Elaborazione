/****************************************Copyright (c)****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD/GLCD.h"
#include "Game/game.h"
#include "LPC17xx.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "joystick/joystick.h"
#include "timer/timer.h"

#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag;  // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

extern GAME_STATE game_state;

extern MOVING_ENTITY moving_entity;
extern uint8_t current_player;

extern struct Player player0;
extern struct Player player1;
extern struct Wall wall;

extern DIRECTION direction;
	
int main(void) {
	SystemInit(); /* System Initialization (i.e., PLL)  */
	LPC_SC->PCON |= 0x1; /* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);
	Peripheral_Init();

  while (1) {
		switch(game_state){
			case TITLE_SCREEN:
				Peripheral_Disable();
				Title_Screen();
				game_state = IDLE;
				break;
			
			case START_GAME:
				LCD_Clear(GameBG);
				Player_Init();
				Board_Init();
				UI_Init();
				Peripheral_Enable();
				game_state = (moving_entity == PLAYER) ? MOVING_PLAYER : MOVING_WALL;
				break;
			
			case CHANGE_MOVING_ENTITY:
				moving_entity = (moving_entity == PLAYER) ? WALL : PLAYER; //if moving player then move wall and vice versa
				game_state = (moving_entity == PLAYER) ? MOVING_PLAYER : MOVING_WALL;
				break;
			
			case MOVING_PLAYER:
				Create_Hint_Move(current_player == 0 ? player0 : player1);
				game_state = IDLE;
				break; 
			
			case MOVING_WALL:
				Remove_Hint_Move(current_player == 0 ? player0 : player1);
				moving_entity = WALL;
				wall = Create_Wall();
				game_state = IDLE;
				break; 
			
			case MOVE_PLAYER:
				moving_entity = PLAYER;
				Move_Player(current_player == 0 ? player0 : player1, direction, 0);
				game_state = CHECK_WIN_CONDITION;
				break; 
			
			case MOVE_WALL:
				moving_entity = WALL;
				wall = Move_Wall(wall, direction);
				game_state = IDLE;
				break; 
			
			case ROTATE_WALL:
				moving_entity = WALL;
				wall = Rotate_Wall(wall);
				game_state = IDLE;
				break;
			
			case CHECK_WIN_CONDITION:
				//if P0 at the top of the board or P1 at the bottom game finished
				game_state = ((player0.pos.y == 0) || (player1.pos.y == NUM_ROWS-1)) ? GAME_OVER : END_TURN;
				break;
			
			case PLACE_WALL: 
				Place_Wall(wall);
				game_state = END_TURN;
				break;
			
			case TIMER_END_TURN:
				if(moving_entity == PLAYER){
					Remove_Hint_Move(current_player == 0 ? player0 : player1);
				} else {
					Remove_Wall(wall);
				}
				game_state = END_TURN;
				break;
			
			case END_TURN:
				End_Turn();
				game_state = MOVE_PLAYER;
				break;
			
			case GAME_OVER:
				Game_Over();
				game_state = TITLE_SCREEN;
				break;
			
			case IDLE:
				__ASM("wfi");
			break;
			
			//default:
			//	GUI_Text(0,0, (uint8_t *)"ERROR: GAME STATE NOT VALID", Black, White);
		}
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
