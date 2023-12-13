#include "button.h"
#include "lpc17xx.h"

#include "../led/led.h"
#include "../timer/timer.h"
#define MAX_LENGTH 100

uint8_t vett_input[MAX_LENGTH]= {0,0,0,0,2,1,1,1,2,0,1,0,0,2,0,1,3,1,1,2,0,0,1,2,1,0,2,1,0,0,2,1,1,1,3,0,1,1,1,1,2,0,0,1,1,1,4};
uint8_t vett_output[MAX_LENGTH];
// 	dot=0
// 	dash=1
// 	endchar=2
// 	space=3
//	end=4
	
void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	LED_Out(0x0);
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	uint8_t curr_char = 0;
	uint8_t i = 0;
  LED_Out(0x0);
	do{
		curr_char = vett_input[i];
		i++;
	}while(curr_char!=4);
	LED_Out(0xff);
	init_timer(0,0x017D7840 * 3);					/* TIMER0 Initialization              */

	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	if(0){
		LPC_SC->EXTINT &= (1 << 0);     /* disable INT0						         */    
		LPC_SC->EXTINT &= (1 << 1);     /* disable KEY2						         */    
	}
	
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}


