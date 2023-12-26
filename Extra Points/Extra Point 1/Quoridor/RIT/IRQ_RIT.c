/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../Game/game.h"

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

void RIT_IRQHandler (void)
{					
	//If game is processing exit and do nothing
	if(game_state == TRANSISTION){
		goto exit;
	}
	
	/* Joytick J_Select pressed p1.25*/
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		
	}
	
	/* Joytick UP p1.29*/
	else if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		Move(UP);
	}
	
	/* Joytick RIGHT p1.28*/
	else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		Move(RIGHT);
	}
	
	/* Joytick DOWN p1.26*/
	else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		Move(DOWN);
	}
	
	/* Joytick LEFT p1.27*/
	else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		Move(LEFT);
	}
	exit:
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
