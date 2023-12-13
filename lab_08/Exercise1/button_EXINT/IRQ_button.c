#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"
extern unsigned char led_value;

void EINT0_IRQHandler (void)	  
{
	LED_Out(led_value=0);
  LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  
{
  LED_Out(++led_value);
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  
{
	LED_Out(--led_value);
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


