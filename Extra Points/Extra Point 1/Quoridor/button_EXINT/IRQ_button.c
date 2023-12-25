#include "button.h"
#include "lpc17xx.h"
#include "../Game/game.h"

extern int down;
extern uint8_t started;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	if(!started){
		
		started = 1;
		Start_Game();
	}
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	down=1;
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}
