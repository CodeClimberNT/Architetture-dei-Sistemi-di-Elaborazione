#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"
char led_counter = 0;

void EINT0_IRQHandler (void)	  
{
	led_counter = 0;
	LED_Out(led_counter);
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  
{
		led_counter+=1;
  LED_Out(led_counter);

	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
		led_counter-=1;
	LED_Out(led_counter);

  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


