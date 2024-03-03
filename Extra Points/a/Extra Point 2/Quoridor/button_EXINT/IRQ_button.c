#include "button.h"
#include "lpc17xx.h"
#include "../RIT/RIT.h"
#include "../Game/game.h"

/*debouncing variable*/
extern uint8_t down_0;
extern uint8_t down_1;
extern uint8_t down_2;

extern uint8_t started;;

void EINT0_IRQHandler(void) /* INT0														 */
{
	if(started){ //INT0 only if game not started
		LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt         */
		return;
	}
		
	
	down_0 = 1;
	NVIC_DisableIRQ(EINT0_IRQn);							/* disable Button interrupts */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
  
  LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt         */
}

void EINT1_IRQHandler(void) /* KEY1														 */
{
	if(!started){
		LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt         */
		return;
	}
	
	down_1 = 1;
	NVIC_DisableIRQ(EINT1_IRQn);							/* disable Button interrupts */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
  LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt         */
}

void EINT2_IRQHandler(void) /* KEY2														 */
{
	if(!started){
		LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt         */
		return;
	}
	
	down_2 = 1;
	NVIC_DisableIRQ(EINT2_IRQn);							/* disable Button interrupts */
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
	
  LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt         */
}
