/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>

#include "../Game/game.h"
#include "lpc17xx.h"
#include "timer.h"

extern struct UI timer_ui;
extern char time_value[2];
extern uint8_t timeLeft;
extern uint8_t started;
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler(void) {
	if(!started){
		LPC_TIM0->IR = 1; /* clear interrupt flag */
		return;
	}
  if (timeLeft > 0 ) {
    Update_Timer_UI(--timeLeft);
  } else
    Timer_End_Turn();

  LPC_TIM0->IR = 1; /* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void) {
  LPC_TIM1->IR = 1; /* clear interrupt flag */
  return;
}

void TIMER2_IRQHandler(void) {
  LPC_TIM2->IR = 1; /* clear interrupt flag */
  return;
}

void TIMER3_IRQHandler(void) {
  LPC_TIM3->IR = 1; /* clear interrupt flag */
  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
