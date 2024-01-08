/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "../Game/game.h"
#include "RIT.h"
#include "lpc17xx.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;

extern struct Wall wall;

extern uint8_t cooldown;

void RIT_IRQHandler(void) {
  if (cooldown > 0)
    --cooldown;

  // If game is processing exit and do nothing
  if (game_state == TRANSITION || cooldown > 0) {
    LPC_RIT->RICTRL |= 0x1;
    return;
  }

  /* Joytick J_Select pressed p1.25*/
  if ((LPC_GPIO1->FIOPIN & (1 << 25)) == 0) {
		if(moving_entity == WALL){
			wall = Place_Wall(wall);
		} else {
			Remove_Last_Wall(wall);
		}
  }

  /* Joytick UP p1.29*/
  else if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
    Move(UP);
  }

  /* Joytick RIGHT p1.28*/
  else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
    Move(RIGHT);
  }

  /* Joytick DOWN p1.26*/
  else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {
    Move(DOWN);
  }

  /* Joytick LEFT p1.27*/
  else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {
    Move(LEFT);
  }

  LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
}
/******************************************************************************
**                            End Of File
******************************************************************************/
